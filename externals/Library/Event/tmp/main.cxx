#ifdef __APPLE__
#include "KqueueManager.hxx"
#else
#include "EpollManager.hxx"
#endif
#include "Item.hxx"
#include "TcpSocket.hxx"
#include "Logging.hxx"

#include <stdio.h>
#include <unistd.h>

void HandleWriteable(const Event::Item& item);
void HandleReadable(const Event::Item& item);
void HandleHangup(const Event::Item& item);
void HandleError(const Event::Item& item);

#ifdef __APPLE__
Event::KqueueManager manager;
#else
Event::EpollManager manager;
#endif

void
HandleWriteable(const Event::Item& item) {
	printf("Socket is writeable\n");

	Network::TcpSocket* socket;
	socket = static_cast<Network::TcpSocket*>(item.GetSocket());
	char data[] = "HTTP/1.0 200 OK";
	size_t bw = sizeof(data);
	socket->Write(data, &bw);

	return;
}

void
HandleReadable(const Event::Item& item) {
	Logging::Info("Socket is readable");

	Network::TcpSocket* socket;
	socket = static_cast<Network::TcpSocket*>(item.GetSocket());
	char data[1024];
	size_t br = sizeof(data);
	socket->Read(data, &br);

	//manager.AddSocket(socket, Event::Readable, NULL);
	socket->Close();
	delete(socket);

	return;
}

void
HandleAcceptable(const Event::Item& item) {
	Logging::Info("Socket is accepting");
	Network::TcpSocket* socket;
	socket = static_cast<Network::TcpSocket*>(item.GetSocket());

	Network::TcpSocket* newsock;
	if (NULL != (newsock = socket->Accept())) {
		Logging::Info("Adding socket to event queue\n");
		manager.AddSocket(newsock, Event::Readable, NULL);
	}
	Logging::Info("Adding accept socket back to queue\n");
	manager.AddSocket(socket, Event::Readable, NULL);

	return;
}

void
HandleHangup(const Event::Item& item) {
	printf("Socket is hanging up\n");
	Network::TcpSocket* socket;
	socket = static_cast<Network::TcpSocket*>(item.GetSocket());
	socket->Close();
	return;
}

void
HandleError(const Event::Item& item) {
	printf("Socket has error\n");
	Network::TcpSocket* socket;
	socket = static_cast<Network::TcpSocket*>(item.GetSocket());
	socket->Close();
	return;
}

int main() {

	//Logging::SetLogLevel(Logging::LevelDebug);
	//Logging::SetTrace(true);

	Logging::Info("Creating Event::EpollManager");

	Logging::Info("Creating Network::Socket");
	Network::TcpSocket* socket = new Network::TcpSocket();
	socket->SetReuseAddr(true);
	socket->Bind("0.0.0.0", 5000);
	socket->Listen(64);

	Logging::Info("Adding socket to manager");
	manager.AddSocket(socket, Event::Readable, (void*)666);

	int num_events;
	bool running = true;
	while (running) {
		num_events = manager.Wait(1000);
		//printf("num_events: [%d]\n", num_events);
		while (num_events > 0) {
			Event::Item item = manager.NextEvent();
			manager.RemoveSocket(item.GetSocket());
			--num_events;
			if (item.IsWriteable()) {
				HandleWriteable(item);
			}
			if (item.IsReadable()) {
				HandleReadable(item);
			}
			if (item.IsAcceptable()) {
				HandleAcceptable(item);
			}
			if (item.IsHangup()) {
				HandleHangup(item);
			}
			if (item.IsError()) {
				HandleError(item);
			}
		}
	}

	return 0;
}
