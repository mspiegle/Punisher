#include "TcpSocket.hxx"
#include "Logging.hxx"
#include "Host.hxx"
#include "HostAddress.hxx"
#include "SocketMap.hxx"
#include "SocketFactory.hxx"

#include <stdio.h>
#include <string.h>

int main() {

	Network::TcpSocket sock;

	//some basic tests
	printf("We just created a TcpSocket.  Here's what we get:\n");
	printf("FD [%d]\n", sock.GetFd());
	printf("Notes [%p]\n", sock.GetNotes());
	printf("Blocking [%s]\n", (true == sock.GetBlocking()) ? "true" : "false");
	printf("\n");

	//socket creation
	printf("Calling Network::TcpSocket.Create(Network::Default)\n");
	if (!sock.Create(Network::Default)) {
		printf("Error creating socket\n");
	}
	printf("FD [%d]\n", sock.GetFd());
	sock.Close();
	printf("\n");


	//test resolver
	printf("Resolver Test:\n");
	Network::Host myhost;
	if (myhost.SetName("nauticaltech.com")) {
		printf("Resolve nauticaltech.com:: %s\n",
		       myhost.Address().ToString().ToCString());
	} else {
		printf("SetName() failure\n");
	}
	printf("\n");

	//test Host stuff
	printf("Host.ToString() Test\n");
	Network::Host myhost2;
	myhost2.SetAddress("1.2.3.4");
	//myhost2.SetPort(80);
	printf("myhost2.ToString(): %s\n", myhost2.ToString().ToCString());
	printf("\n");

	//test server connection
	printf("Test server connection\n");
	Network::TcpSocket con_test_sock;
	if (!con_test_sock.SetBlocking(true)) {
		printf("Error setting blocking\n");
	}
	printf("Blocking: [%s]\n",
	       (true == con_test_sock.GetBlocking()) ? "true" : "false");
	Network::Host nt;
	nt.SetPort(80);
	if (nt.SetName("nauticaltech.com")) {
		printf("Resolved nt to %s\n", nt.Address().ToString().ToCString());
	} else {
		printf("Issue resolving nt\n");
		return 0;
	}
	Network::network_error_t error;
	Logging::SetLogLevel(Logging::LEVEL_DEBUG);
	if (Network::NETWORK_SUCCESS == (error = con_test_sock.Connect(nt))) {
		printf("Sucessfully connected\n");
	} else {
		printf("Error [%d] Connecting\n", error);
	}
	con_test_sock.Close();
	printf("\n");

	//reference test
	printf("Reference test:\n");
	Network::Host reftest1;
	reftest1.SetName("nauticaltech.com");
	Network::Host& reftest2 = reftest1;
	if (0 == memcmp(&reftest1, &reftest2, sizeof(reftest1))) {
		printf("reftest: identical\n");
	} else {
		printf("reftest: different\n");
	}

	//SocketMap testing
	Network::SocketMap map;
	Network::Socket* mapsocket = NULL;
	mapsocket = Network::SocketFactory::CreateSocket(Network::SOCKET_TCP);
	printf("fd of mapsocket [%d]\n", mapsocket->GetFd());
	map.Push(mapsocket);
	Network::Socket* mapsocket2 = NULL;
	mapsocket2 = map.Pop();
	printf("fd of mapsocket2 [%d]\n", mapsocket2->GetFd());
	Network::Socket* mapsocket3 = (Network::Socket*)0x1;
	printf("addr should be 0x1 [%p]\n", mapsocket3);
	mapsocket3 = map.Pop();
	printf("addr should be null [%p]\n", mapsocket3);

	return 0;
}
