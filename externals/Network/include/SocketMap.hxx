/*
 * SocketMap
 * by Michael Spiegle
 * 02.03.12
 *
 * SocketMap.hxx
 */

#ifndef _NETWORK_SOCKET_MAP_HXX_
#define _NETWORK_SOCKET_MAP_HXX_

#include "Socket.hxx"
#include <pthread.h>
#include <deque>

namespace Network {

class SocketMap {
	private:
		std::deque<Socket*> container;
		pthread_mutex_t lock;

	public:
		SocketMap();
		~SocketMap();
		void Init();

		void Push(Socket* socket);
		Socket* Pop();

		int Length();
};

}

#endif
