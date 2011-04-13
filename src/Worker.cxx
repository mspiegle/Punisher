/*
 * Punisher
 * by Michael Spiegle
 * 10.12.09
 *
 * Worker.cxx
 */

#include "Config.hxx"
#include "IHandler.hxx"
#include "PollManager.hxx"
#include "Worker.hxx"
#include "Item.hxx"
#include "Request.hxx"
#include "Logging.hxx"
#include "Protocol.hxx"
#include "Network.hxx"
#include "Socket.hxx"
#include "Statistics.hxx"

#include <unistd.h>
#include <stdio.h>

namespace Punisher {

Worker::Worker() {
	next_request = 0;
	iterations = (size_t)-1;
	config = NULL;
}

Worker::~Worker() {
}

const Statistics&
Worker::GetStatistics() const {
	return stats;
}

const Config*
Worker::GetConfig() const {
	return config;
}
void
Worker::SetConfig(const Config* config) {
	if (config != NULL) {
		this->config = config;
	}
}

void
Worker::SetIterations(size_t iterations) {
	if (0 == iterations) {
		iterations = (size_t)-1;
	} else {
		this->iterations = iterations;
	}
}

void*
Worker::ThreadMain() {
	while (true == GetRunning()) {
		//everytime we loop, make sure that we have enough sockets open
		while (stats.GetOpenSockets() < config->GetConnectionsPerThread()) {
			LOGGING_DEBUG("Worker::ThreadMain(): socket check: while (%d < %d)",
			              stats.GetOpenSockets(), config->GetConnectionsPerThread());

			//grab the next request and create socket
			const Request* request = config->GetRequest(id, next_request);
			Network::Socket* socket = NULL;

			/*
			//TODO: keepalives break my ability to manage multiple protocols
			//create the appropriate socket or get a keepalive socket
			if (config->GetKeepalive()) {
				if (keepalives.size() > 1) {
					socket = keepalives.front();
					keepalives.pop_front();
				}
			}
			*/
		
			//if there were no keepalive sockets available...
			if (NULL == socket) {
				socket = request->CreateSocket();
				stats.AddOpenSockets(1);
			}

			if (!socket->SetBlocking(false)) {
				Logging::Error("Worker::ThreadMain(): Error during SetBlocking()");
			}

			LOGGING_DEBUG("Worker::ThreadMain(): Calling socket->SetReuseAddr()");
			if (!socket->SetReuseAddr(true)) {
				Logging::Error("Worker::ThreadMain(): Error during SetReuseAddr()");
			}

			//create a protocol that matches the type of the request
			Protocol* protocol = request->CreateProtocol();

			// do the initial connect (which probably returns EWOULDBLOCK)
			// but we'll check just to make sure
			Network::network_error_t status;
			if (Network::NETWORK_SUCCESS == (status = protocol->Connect(socket))) {
				stats.AddConnectedSockets(1);
			}

			// if we get an error here, then something bad happened.  just delete it
			if (Network::NETWORK_ERROR == status) {
				Logging::Error("Worker::ThreadMain(): error connecting socket");
				delete(socket);
				delete(protocol);
				continue;
			}

			// create protocol validator that matches request validator
			if (NULL != request->GetValidator()) {
				protocol->SetValidator(request->GetValidator()->CreateValidator());
			}

			// stick the socket in the queue
			manager.AddSocket(socket, Event::Writeable, (void*)protocol);
		}

		//TODO: I think there's a way to factor some of this out into the
		//::Event namespace.
		//wait for, and process events as they come back
		int num_events = 0;
		num_events = manager.Wait(1000);
		LOGGING_DEBUG("Worker::ThreadMain(): [%d] events to process", num_events);
		for (int x = 0; x < num_events; x++) {
			Event::Item item = manager.NextEvent();
			manager.RemoveSocket(item.GetSocket());
			
			//If the socket isn't connected, you'll get EPOLLHUP regardless
			if (item.IsHangup() && item.GetSocket()->GetConnected()) {
				LOGGING_DEBUG("Worker::ThreadMain(): item is hangup");
				HandleHangup(item);
			} else
			if (item.IsError()) {
				LOGGING_DEBUG("Worker::ThreadMain(): item is error");
				HandleError(item);
			} else
			if (item.IsWriteable()) {
				LOGGING_DEBUG("Worker::ThreadMain(): item is writeable");
				HandleWriteable(item);
			} else
			if (item.IsReadable()) {
				LOGGING_DEBUG("Worker::ThreadMain(): item is hangup");
				HandleReadable(item);
			} else {
				Logging::Error("Odd event received");
			}
		}
		
		//we did what was asked of us.  we can now stop
		if (stats.GetTotalRequests() >= iterations) {
			SetRunning(false);
		}
	}
	return NULL;
}

void
Worker::HandleReadable(const Event::Item& item) {
	LOGGING_DEBUG("Worker::HandleReadable()");
	
	Network::Socket* socket = item.GetSocket();

	Protocol* protocol;
	protocol = static_cast<Protocol*>(item.GetUserData());

	protocol_result_t ret = protocol->ReadData(socket);

	if (true == ret.success) {
		stats.AddBytesReceived(ret.bytes_transferred);
		switch (ret.state) {
			case STATE_DONE:
				// here, we collect per-request metrics
				LOGGING_DEBUG("Adding usec[%lu]", protocol->GetRequestDuration());
				stats.AddRequestDuration(protocol->GetRequestDuration());

				// clean up protocol
				delete(protocol);

				//handle keepalives
				LOGGING_DEBUG("Worker::HandleReadable(): testing keepalive");
				if (config->GetKeepalive() && (ret.keepalive == true)) {
					LOGGING_DEBUG("Worker::HandleReadable(): keepalive socket");
					keepalives.push_back(socket);
				} else {
					delete(socket);
					stats.AddOpenSockets(-1);
					stats.AddConnectedSockets(-1);
				}

				stats.AddTotalRequests(1);
				break;

			case STATE_READ:
				manager.AddSocket(socket, Event::Readable, (void*)protocol);
				break;

			case STATE_WRITE:
				manager.AddSocket(socket, Event::Writeable, (void*)protocol);
				break;

			default:
				break;
		}
		return;
	}

	LOGGING_DEBUG("Worker::HandleReadable(): protocol returned failure");
	errors.push_front(protocol->GetError());
	delete(protocol);
	delete(socket);
	stats.AddConnectedSockets(-1);
	stats.AddOpenSockets(-1);
	stats.AddTotalRequests(1);
	stats.AddFailedRequests(1);
}

void
Worker::HandleWriteable(const Event::Item& item) {
	LOGGING_DEBUG("Worker::HandleWriteable()");

	Network::Socket* socket = item.GetSocket();

	Protocol* protocol;
	protocol = static_cast<Protocol*>(item.GetUserData());

	if (socket->GetConnected()) {
		//already connected.  we can write data
		LOGGING_DEBUG("Worker::HandleWriteable(): Already connected, sending...");

		protocol_result_t ret = protocol->WriteData(socket);

		if ((true == ret.success) && (STATE_READ == ret.state)) {
			stats.AddBytesSent(ret.bytes_transferred);
			manager.AddSocket(socket, Event::Readable, (void*)protocol);
			return;
		}

		Logging::Error("Worker::HandleWriteable(): protocol result unsuccessful");
		if (false == ret.success) {
			errors.push_front(protocol->GetError());
		}
		stats.AddTotalRequests(1);
		stats.AddFailedRequests(1);
	} else {
		//we'll handle the connect right here
		Network::network_error_t status;
		if (Network::NETWORK_SUCCESS == (status = protocol->Connect(socket))) {
			stats.AddConnectedSockets(1);
			manager.AddSocket(socket, Event::Writeable, (void*)protocol);
			return;
		}

		if (Network::NETWORK_WOULDBLOCK == status) {
			LOGGING_DEBUG("Worker::HandleWriteable(): The connect would block");
			manager.AddSocket(socket, Event::Writeable, (void*)protocol);
			return ;
		}
		
		Logging::Error("Worker::HandleWriteable(): Error during Connect");
	}

	delete(protocol);
	delete(socket);
	stats.AddOpenSockets(-1);
	stats.AddConnectedSockets(-1);
}

void
Worker::HandleHangup(const Event::Item& item) {
	Logging::Error("HandleHangup()");

	Network::Socket* socket = item.GetSocket();
	Protocol* protocol = static_cast<Protocol*>(item.GetUserData());

	//if we were connected, then we should bump the request counters
	//if we weren't connected, we only need to handle the socket counters
	if (socket->GetConnected()) {
		stats.AddTotalRequests(1);
		stats.AddFailedRequests(1);
		stats.AddConnectedSockets(-1);
	}

	delete(protocol);
	delete(socket);
	stats.AddOpenSockets(-1);
}

void
Worker::HandleError(const Event::Item& item) {
	Logging::Error("HandleError()");

	Network::Socket* socket = item.GetSocket();
	Protocol* protocol = static_cast<Protocol*>(item.GetUserData());

	//if we were connected, then we should bump the request counters
	//if we weren't connected, we only need to handle the socket counters
	if (socket->GetConnected()) {
		stats.AddTotalRequests(1);
		stats.AddFailedRequests(1);
		stats.AddConnectedSockets(-1);
	}

	delete(protocol);
	delete(socket);
	stats.AddOpenSockets(-1);
}

void
Worker::HandleAcceptable(const Event::Item& item) {
	//this method is only here to satisfy the interface
	//it will never get called
	(void)item;
}

}
// vim:ts=2
