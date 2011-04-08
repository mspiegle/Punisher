/*
 * Event
 * by Michael Spiegle
 * 03.29.11
 *
 * PollManager.hxx
 */

#ifndef _EVENT_POLLMANAGER_HXX_
#define _EVENT_POLLMANAGER_HXX_

#if defined(__linux__)
#include "EpollManager.hxx"
namespace Event {
typedef EpollManager PollManager;
}
#elif defined(__APPLE__)
#include "KqueueManager.hxx"
namespace Event {
typedef KqueueManager PollManager;
}
#endif

#endif
