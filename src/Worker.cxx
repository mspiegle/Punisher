/*
 * Punisher
 * by Michael Spiegle
 * 10.12.09
 *
 * Worker.cxx
 */

#include "Config.hxx"
#include "IHandler.hxx"
#include "EpollManager.hxx"
#include "Worker.hxx"
#include "Item.hxx"
#include "Request.hxx"
#include "Logging.hxx"
#include "State.hxx"
#include "Network.hxx"
#include "Socket.hxx"
#include "Statistics.hxx"

#include <unistd.h>
#include <stdio.h>

namespace Punisher {

Worker::Worker() {
	total_requests = 0;
	failed_requests = 0;
	open_sockets = 0;
	connected_sockets = 0;
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
		LOGGING_DEBUG("Worker::ThreadMain(): do we have enough sockets?");
		while (open_sockets < (config->GetConcurrent() / config->GetThreads())) {
			LOGGING_DEBUG("while (%d < (%d / %d))", open_sockets, 
			              config->GetConcurrent(),
			              config->GetThreads());

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
			}

			if (!socket->SetBlocking(false)) {
				Logging::Error("Worker::ThreadMain(): Error during SetBlocking()");
			}
			if (!socket->SetReuseAddr(true)) {
				Logging::Error("Worker::ThreadMain(): Error during SetReuseAddr()");
			}

			//create a state that matches the type of the request
			State* state = request->CreateState();

			//create a state validator that matches the type of the request validator
			if (NULL != request->GetValidator()) {
				state->GetValidator() = request->GetValidator()->CreateValidator();
			}

			manager.AddSocket(socket, Event::Writeable, (void*)state);
			open_sockets++;
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
				HandleHangup(item);
			} else
			if (item.IsError()) {
				HandleError(item);
			} else
			if (item.IsWriteable()) {
				HandleWriteable(item);
			} else
			if (item.IsReadable()) {
				HandleReadable(item);
			} else {
				Logging::Error("Odd event received");
			}
		}
		
		//we did what was asked of us.  we can now stop
		if (total_requests >= iterations) {
			SetRunning(false);
		}
	}
	return NULL;
}

void
Worker::HandleReadable(const Event::Item& item) {
	LOGGING_DEBUG("Worker::HandleReadable()");
	
	Network::Socket* socket = item.GetSocket();

	State* state;
	state = static_cast<State*>(item.GetUserData());

	state_result_t ret = state->ReadData(socket);

	if (true == ret.success) {
		stats.AddBytesReceived(ret.bytes_transferred);
		switch (ret.mode) {
			case MODE_DONE:
				delete(state);

				//handle keepalives
				LOGGING_DEBUG("Worker::HandleReadable(): testing keepalive");
				if (config->GetKeepalive() && (ret.keepalive == true)) {
					LOGGING_DEBUG("Worker::HandleReadable(): keepalive socket");
					keepalives.push_back(socket);
				} else {
					delete(socket);
					--connected_sockets;
					--open_sockets;
				}

				++total_requests;
				break;

			case MODE_READ:
				manager.AddSocket(socket, Event::Readable, (void*)state);
				break;

			case MODE_WRITE:
				manager.AddSocket(socket, Event::Writeable, (void*)state);
				break;

			default:
				break;
		}
		return;
	}

	LOGGING_DEBUG("Worker::HandleReadable(): state returned failure");
	errors.push_front(state->GetError());
	delete(state);
	delete(socket);
	--connected_sockets;
	--open_sockets;
	++total_requests;
	++failed_requests;
}

void
Worker::HandleWriteable(const Event::Item& item) {
	LOGGING_DEBUG("Worker::HandleWriteable()");

	Network::Socket* socket = item.GetSocket();

	State* state;
	state = static_cast<State*>(item.GetUserData());

	if (socket->GetConnected()) {
		//already connected.  we can write data
		LOGGING_DEBUG("Worker::HandleWriteable(): Already connected, sending...");

		state_result_t ret = state->WriteData(socket);

		if ((true == ret.success) && (MODE_READ == ret.mode)) {
			stats.AddBytesSent(ret.bytes_transferred);
			manager.AddSocket(socket, Event::Readable, (void*)state);
			return;
		}

		Logging::Error("Worker::HandleWriteable(): state result unsuccessful");
		if (false == ret.success) {
			errors.push_front(state->GetError());
		}
		++total_requests;
		++failed_requests;
	} else {
		//we'll handle the connect right here
		Network::network_error_t status;
		if (Network::NETWORK_SUCCESS == (status = state->Connect(socket))) {
			connected_sockets++;
			manager.AddSocket(socket, Event::Writeable, (void*)state);
			return;
		}

		if (Network::NETWORK_WOULDBLOCK == status) {
			LOGGING_DEBUG("Worker::HandleWriteable(): The connect would block");
			manager.AddSocket(socket, Event::Writeable, (void*)state);
			return ;
		}
		
		Logging::Error("Worker::HandleWriteable(): Error during Connect");
	}

	delete(state);
	delete(socket);
	--open_sockets;
	--connected_sockets;
}

void
Worker::HandleHangup(const Event::Item& item) {
	Logging::Error("HandleHangup()");

	Network::Socket* socket = item.GetSocket();
	State* state = static_cast<State*>(item.GetUserData());

	//if we were connected, then we should bump the request counters
	//if we weren't connected, we only need to handle the socket counters
	if (socket->GetConnected()) {
		++total_requests;
		++failed_requests;
		--connected_sockets;
	}

	delete(state);
	delete(socket);
	--open_sockets;
}

void
Worker::HandleError(const Event::Item& item) {
	Logging::Error("HandleError()");

	Network::Socket* socket = item.GetSocket();
	State* state = static_cast<State*>(item.GetUserData());

	//if we were connected, then we should bump the request counters
	//if we weren't connected, we only need to handle the socket counters
	if (socket->GetConnected()) {
		++total_requests;
		++failed_requests;
		--connected_sockets;
	}

	delete(state);
	delete(socket);
	--open_sockets;
}

void
Worker::HandleAcceptable(const Event::Item& item) {
	//this method is only here to satisfy the interface
	//it will never get called
	(void)item;
}

}
// vim:ts=2
