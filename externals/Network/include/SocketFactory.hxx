/*
 * SocketFactory
 * by Michael Spiegle
 * 04.21.11
 *
 * SocketFactory.hxx
 */

#ifndef _NETWORK_SOCKETFACTORY_HXX_
#define _NETWORK_SOCKETFACTORY_HXX_

#include "Network.hxx"
#include "Socket.hxx"

namespace Network {

class SocketFactory {
	public:
		static Socket* CreateSocket(socket_type_t type);
};

}

#endif
