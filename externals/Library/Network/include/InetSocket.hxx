/*
 * Socket
 * by Michael Spiegle
 * 09.18.09
 *
 * InetSocket.h
 */

#ifndef _NETWORK_INETSOCKET_H_
#define _NETWORK_INETSOCKET_H_

#include "Socket.hxx"
#include "Host.hxx"

#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <netinet/in.h>

namespace Network {

class InetSocket : public Socket {
	public:
		InetSocket();
		~InetSocket();

		bool Create(int type, int protocol);
		InetSocket* Accept();

		network_error_t Connect(const struct sockaddr_in& saddr, socklen_t size);
		network_error_t Connect(const struct in_addr& addr, const uint16_t port);
		network_error_t Connect(const char* ip, const uint16_t port);
		network_error_t Connect(const M::String& ip, const uint16_t port);
		network_error_t Connect(const Host& host);

		bool Bind(const struct sockaddr_in& saddr, socklen_t size);
		bool Bind(const struct in_addr& addr, uint16_t port);
		bool Bind(const char* ip, uint16_t port);
		bool Bind(const M::String& ip, uint16_t port);
};

}

#endif
