/*
 * Event
 * by Michael Spiegle
 * 10.06.09
 *
 * IHandler.hxx
 */

#ifndef _EVENT_IHANDLER_HXX_
#define _EVENT_IHANDLER_HXX_

#include "Item.hxx"

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
