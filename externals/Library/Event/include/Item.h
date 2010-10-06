/*
 * Event
 * by Michael Spiegle
 * 09.16.09
 *
 * Item.h
 */

#ifndef _EVENT_ITEM_H_
#define _EVENT_ITEM_H_

#include <stdint.h>
#include <Socket.h>

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
