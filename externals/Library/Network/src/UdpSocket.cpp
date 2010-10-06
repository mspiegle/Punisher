/*
 * Socket
 * by Michael Spiegle
 * 09.07.09
 *
 * UdpSocket.cpp
 */

#include "InetSocket.h"
#include "UdpSocket.h"

namespace Network {

UdpSocket::UdpSocket() { }
UdpSocket::~UdpSocket() { }

bool
UdpSocket::Create(int protocol) {
	return InetSocket::Create(Network::Datagram, protocol);
}

UdpSocket*
UdpSocket::Accept() {
	return static_cast<UdpSocket*>(InetSocket::Accept());
}

}
