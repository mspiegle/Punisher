/*
 * SocketFactory
 * by Michael Spiegle
 * 04.21.11
 *
 * SocketFactory.hxx
 */

#include "Network.hxx"
#include "SocketFactory.hxx"
#include "TcpSocket.hxx"
#include "UdpSocket.hxx"
#include "UnixSocket.hxx"

namespace Network {

Socket*
SocketFactory::CreateSocket(socket_type_t type) {
	switch(type) {
		case SOCKET_TCP:
			return new TcpSocket();

		case SOCKET_UDP:
			return new UdpSocket();

		case SOCKET_UNIX:
			return new UnixSocket();
	}
	return NULL;
}

}
