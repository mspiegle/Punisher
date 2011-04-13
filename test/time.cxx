#include "TcpSocket.hxx"
#include "HostAddress.hxx"

#include <stdio.h>
#include <sys/time.h>
#include <stdint.h>

int main() {
	// time objects
	struct timeval start_time;
	struct timeval end_time;


	// create a host object
	Network::Host test_host;
	test_host.SetName("nauticaltech.com");
	test_host.SetPort(80);

	// create a tcp socket
	Network::TcpSocket con;
	con.SetBlocking(true);
	
	// start time
	gettimeofday(&start_time, NULL);

	// make connection
	Network::network_error_t ret;
	ret = con.Connect(test_host);
	if (Network::NETWORK_SUCCESS != ret) {
		printf("Connect failure\n");
	}

	// send request
	const char request[] = "GET / HTTP/1.0\r\nHost: www.nauticaltech.com\r\n\r\n";
	size_t bw = strlen(request);
	con.Write(request, &bw);

	// receive response
	char buffer[4096];
	size_t br = 4096;
	con.Read(buffer, &br);

	//buffer[br + 1] = '\0';
	//printf("Response\n%s\n" , buffer);


	// end time
	gettimeofday(&end_time, NULL);

	uint64_t start_usec = (start_time.tv_sec * 1000000) + start_time.tv_usec;
	uint64_t end_usec = (end_time.tv_sec * 1000000) + end_time.tv_usec;
	printf("usec[%llu]\n", end_usec - start_usec);
	
	con.Close();

	return 0;
}
