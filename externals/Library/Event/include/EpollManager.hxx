/*
 * Event
 * by Michael Spiegle
 * 09.16.09
 *
 * EpollManager.hxx
 */

#ifndef _EVENT_EPOLLMANAGER_HXX_
#define _EVENT_EPOLLMANAGER_HXX_

#include "Manager.hxx"
#include "Socket.hxx"
#include "Item.hxx"

#include <stdint.h>
#include <queue>
#include <sys/epoll.h>

const int max_events = 64;

namespace Event {

class EpollManager : public Manager {
	private:
		int efd;
		epoll_event event_list[max_events];
		uint32_t LocalToSystem(uint32_t filter_in);
		uint32_t SystemToLocal(uint32_t filter_in);


	public:
		EpollManager();
		~EpollManager();

		int Wait(int timeout);
		bool AddSocket(Network::Socket* socket, uint32_t filter,
		               void* user_data);
		bool RemoveSocket(Network::Socket* socket);
};

}

#endif
