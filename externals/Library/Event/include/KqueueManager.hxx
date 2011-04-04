/*
 * Event
 * by Michael Spiegle
 * 09.16.09
 *
 * KqueueManager.hxx
 */

#ifndef _EVENT_KQUEUEMANAGER_HXX_
#define _EVENT_KQUEUEMANAGER_HXX_

#include "Manager.hxx"
#include "Socket.hxx"
#include "Item.hxx"

#include <stdint.h>
#include <sys/event.h>

const int max_events = 64;

namespace Event {

class KqueueManager : public Manager {
	private:
		int kfd;
		struct kevent event_list[max_events];
		int16_t LocalToSystem(int16_t filter_in);
		int16_t SystemToLocal(int16_t filter_in);


	public:
		KqueueManager();
		~KqueueManager();

		int Wait(int timeout);
		bool AddSocket(Network::Socket* socket, uint32_t filter, void* user_data);
		bool RemoveSocket(Network::Socket* socket);
};

}

#endif
