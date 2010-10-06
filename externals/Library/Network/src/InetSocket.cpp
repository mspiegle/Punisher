/*
 * Socket
 * by Michael Spiegle
 * 09.18.09
 *
 * InetSocket.cpp
 */

#include "InetSocket.h"
#include "Logging.h"
#include "Host.h"
#include "String.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>

namespace Network {

InetSocket::InetSocket() {
	LOGGING_DEBUG("InetSocket::InetSocket()");
}

InetSocket::~InetSocket() {
	LOGGING_DEBUG("InetSocket::~InetSocket()");
}

bool
InetSocket::Create(int type, int protocol) {
	LOGGING_DEBUG("InetSocket::Create()");
	return Socket::Create(Network::Inet, type, protocol);
}

InetSocket*
InetSocket::Accept() {
	LOGGING_DEBUG("InetSocket::Accept()");
	return static_cast<InetSocket*>(Socket::Accept());
}

bool
InetSocket::Bind(const struct sockaddr_in& saddr, socklen_t size) {
	return Socket::Bind((const struct sockaddr*)&saddr, size);
}

bool
InetSocket::Bind(const struct in_addr& addr, const uint16_t port) {
	struct sockaddr_in saddr;
	memset(&saddr, 0, sizeof(saddr));

	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	memcpy(&(saddr.sin_addr), &addr, sizeof(struct in_addr));

	return InetSocket::Connect(saddr, sizeof(saddr));
}

bool
InetSocket::Bind(const char* ip, const uint16_t port) {
	struct in_addr addr;
	memset(&addr, 0, sizeof(addr));

	if (0 == inet_aton(ip, &addr)) {
		Logging::Error("InetSocket::Bind(): Invalid IP Address");
		return false;
	}

	return InetSocket::Bind(addr, port);
}

bool
InetSocket::Bind(const M::String& ip, const uint16_t port) {
	return InetSocket::Bind(ip.ToCString(), port);
}

network_error_t
InetSocket::Connect(const struct sockaddr_in& saddr, socklen_t size) {
	LOGGING_DEBUG("InetSocket::Connect(struct sockaddr_in, socklen_t)");
	return Socket::Connect((const struct sockaddr*)&saddr, size);
}

network_error_t
InetSocket::Connect(const struct in_addr& addr, const uint16_t port) {
	LOGGING_DEBUG("InetSocket::Connect([%p], [%u])", addr, port);
	
	struct sockaddr_in saddr;
	memset(&saddr, 0, sizeof(saddr));

	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	memcpy(&(saddr.sin_addr), &addr, sizeof(struct in_addr));

	return InetSocket::Connect(saddr, sizeof(saddr));
}

network_error_t
InetSocket::Connect(const char* ip, const uint16_t port) {
	LOGGING_DEBUG("InetSocket::Connect([%s], [%u])", ip, port);

	struct in_addr addr;
	memset(&addr, 0, sizeof(addr));
	
	if (0 == inet_aton(ip, &addr)) {
		Logging::Error("InetSocket::Connect(): Invalid IP address");
		return NETWORK_ERROR;
	}

	return InetSocket::Connect(addr, port);
}

network_error_t
InetSocket::Connect(const M::String& ip, const uint16_t port) {
	LOGGING_DEBUG("InetSocket::Connect([%s], [%u])", ip.ToCString(), port);
	return InetSocket::Connect(ip.ToCString(), port);
}

network_error_t
InetSocket::Connect(const Host& host) {
	LOGGING_DEBUG("InetSocket::Connect([%s])", host.ToString().ToCString());
	return InetSocket::Connect(host.Address().Get(), host.Port().Get());
}

}
