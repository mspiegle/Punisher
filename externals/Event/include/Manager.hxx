/*
 * Event
 * by Michael Spiegle
 * 09.16.09
 *
 * Manager.hxx
 */

#ifndef _EVENT_MANAGER_HXX_
#define _EVENT_MANAGER_HXX_

#include "Socket.hxx"
#include "Item.hxx"

#include <stdint.h>
#include <queue>

namespace Event {

class Manager {
	protected:
		std::queue<Item> events;

	public:
		Manager();
		virtual ~Manager();

		virtual int Wait(int timeout) = 0;
		virtual bool AddSocket(Network::Socket* socket, uint32_t events,
		                       void* user_data) = 0;
		virtual bool RemoveSocket(Network::Socket* socket) = 0;

		Item NextEvent();
};

class PrivateData {
	private:
		void*            user_data;
		Network::Socket* socket;

	public:
		PrivateData();
		~PrivateData();

		void* GetUserData() const;
		void  SetUserData(void* user_data);

		Network::Socket* GetSocket() const;
		void             SetSocket(Network::Socket* socket);
};

}

#endif
