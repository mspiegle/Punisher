#ifdef __APPLE__
#include "KqueueManager.hxx"
#else
#include "EpollManager.hxx"
#endif

#include "Logging.hxx"
#include "TcpSocket.hxx"

#include <stdio.h>

int main() {
	//this is going to be interesting, but I'm going to try to create a client
	//and server in the same file here, and they'll talk to each other.
	Logging::SetTrace(true);
	Logging::SetLogLevel(Logging::LEVEL_DEBUG);
	Logging::Info("Starting test suite for Event");

	//create client/server managers
#ifdef __APPLE__
	Event::KqueueManager client_manager;
	Event::KqueueManager server_manager;
#else
	Event::EpollManager client_manager;
	Event::EpollManager server_manager;
#endif

	//create a client socket, and a server listen socket
	//the server socket should be put in the listening state
	Network::TcpSocket* client_socket = new Network::TcpSocket();
	Network::TcpSocket* listen_socket = new Network::TcpSocket();
	if (Network::NETWORK_SUCCESS != listen_socket->Bind("127.0.0.1", 5000)) {
		Logging::Error("Error calling TcpSocket->Bind()");
		return 1;
	}
	if (Network::NETWORK_SUCCESS != listen_socket->Listen(8)) {
		Logging::Error("Error calling TcpSocket->Listen()");
		return 1;
	}

	//stick the sockets in their respective managers
	client_manager.AddSocket(client_socket, Event::Writeable, (void*)0);
	server_manager.AddSocket(listen_socket, Event::Readable, (void*)0);

	//


	return 0;
}
