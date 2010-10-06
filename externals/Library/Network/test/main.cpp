#include "TcpSocket.h"
#include "Logging.h"
#include "Host.h"
#include "HostAddress.h"

#include <stdio.h>
#include <string.h>

int main() {

	Network::TcpSocket sock;

	//some basic tests
	printf("FD [%d]\n", sock.GetFd());
	printf("Notes [%p]\n", sock.GetNotes());
	printf("Blocking [%s]\n", (true == sock.GetBlocking()) ? "true" : "false");

	//socket creation
	if (!sock.Create(Network::Default)) {
		printf("Error creating socket\n");
	}
	printf("FD [%d]\n", sock.GetFd());
	sock.Close();


	//test resolver
	printf("Resolver Test\n");
	Network::Host myhost;
	if (myhost.SetName("nauticaltech.com")) {
		printf("resolved to: %s\n", myhost.Address().ToString().ToCString());
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
	if (!con_test_sock.SetBlocking(false)) {
		printf("Error setting blocking\n");
	}
	printf("Blocking: [%s]\n",
	       (true == con_test_sock.GetBlocking()) ? "true" : "false");
	Network::Host vm02;
	vm02.SetPort(80);
	if (vm02.SetName("vm02")) {
		printf("Resolved vm02 to %s\n", vm02.Address().ToString().ToCString());
	} else {
		printf("Issue resolving vm02\n");
		return 0;
	}
	Network::network_error_t error;
	if (Network::NETWORK_SUCCESS == (error = con_test_sock.Connect(vm02))) {
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

	return 0;
}
