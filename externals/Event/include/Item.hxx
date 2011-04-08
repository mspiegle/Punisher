/*
 * Event
 * by Michael Spiegle
 * 09.16.09
 *
 * Item.hxx
 */

#ifndef _EVENT_ITEM_HXX_
#define _EVENT_ITEM_HXX_

#include "Socket.hxx"

#include <stdint.h>

namespace Event {

enum {
	Readable = 0x01,
	Writeable = 0x02,
	Hangup = 0x04,
	Error = 0x08
};

class Item {
	private:
		uint32_t         filter;
		void*            user_data;
		Network::Socket* socket;

	public:
		Item();
		~Item();

		bool IsWriteable() const;
		bool IsReadable() const;
		bool IsAcceptable() const;
		bool IsHangup() const;
		bool IsError() const;

		void SetFilter(uint32_t filter);

		void* GetUserData() const;
		void  SetUserData(void* user_data);

		Network::Socket* GetSocket() const;
		void             SetSocket(Network::Socket* socket);
};

}

#endif
