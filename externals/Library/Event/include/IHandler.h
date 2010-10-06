/*
 * Event
 * by Michael Spiegle
 * 10.06.09
 *
 * IHandler.h
 */

#ifndef _EVENT_IHANDLER_H_
#define _EVENT_IHANDLER_H_

#include "Item.h"

namespace Event {

class IHandler {
	public:
		inline IHandler() {
		}
		inline virtual ~IHandler() {
		}
		virtual void HandleReadable(const Item& item) = 0;
		virtual void HandleAcceptable(const Item& item) = 0;
		virtual void HandleWriteable(const Item& item) = 0;
		virtual void HandleHangup(const Item& item) = 0;
		virtual void HandleError(const Item& item) = 0;
};

}

#endif
