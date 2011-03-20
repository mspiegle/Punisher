/*
 * Socket
 * by Michael Spiegle
 * 09.07.09
 *
 * TcpSocket.cpp
 */

#include "InetSocket.h"
#include "TcpSocket.h"
#include "Logging.h"

namespace Network {

TcpSocket::TcpSocket() {
	LOGGING_DEBUG("TcpSocket::TcpSocket()");
	Create(0);
}

TcpSocket::TcpSocket(int protocol) {
	LOGGING_DEBUG("TcpSocket::TcpSocket(int protocol)");
	Create(protocol);
}

TcpSocket::~TcpSocket() {
	LOGGING_DEBUG("TcpSocket::~TcpSocket()");
}

bool
TcpSocket::Create(int protocol) {
	return InetSocket::Create(Network::Stream, protocol);
}

TcpSocket*
TcpSocket::Accept() {
	return static_cast<TcpSocket*>(InetSocket::Accept());
}

}
