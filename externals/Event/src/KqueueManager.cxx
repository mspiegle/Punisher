/*
 * Event
 * by Michael Spiegle
 * 09.16.09
 *
 * KqueueManager.cxx
 */

#include "KqueueManager.hxx"
#include "Logging.hxx"

#include <sys/event.h>
#include <unistd.h>
#include <errno.h>

namespace Event {

KqueueManager::KqueueManager() {
	LOGGING_DEBUG("KqueueManager::KqueueManager()");
	kfd = kqueue();
	if (kfd == -1) {
		Logging::Error("KqueueManager::KqueueManager(): kqueue() failure");
	}
}
KqueueManager::~KqueueManager() {
	LOGGING_DEBUG("KqueueManager::~KqueueManager()");
	close(kfd);
}

int
KqueueManager::Wait(int timeout) {
	LOGGING_DEBUG("KqueueManager::Wait(%d)", timeout);
	int num_events = -1;
	struct timespec wait;
	wait.tv_sec = timeout / 1000;
	wait.tv_nsec = (timeout % 1000) * 1000;
	int error;
	num_events = kevent(kfd, NULL, 0, event_list, max_events, &wait);
	if (-1 == num_events) {
		error = errno;
		Logging::Error("KqueueManager::Wait(): kevent(): %s", strerror(error));
		return 0;
	}
	LOGGING_DEBUG("KqueueManager::Wait(%d): kevent(): returned [%d] events",
	              timeout, num_events);
	for (int x = 0; x < num_events; x++) {
		Item item;
		item.SetFilter(SystemToLocal(event_list[x].filter));
		PrivateData* data = static_cast<PrivateData*>(event_list[x].udata);
		item.SetUserData(data->GetUserData());
		item.SetSocket(data->GetSocket());
		delete(data);
		events.push(item);
	}
	return num_events;
}

bool
KqueueManager::AddSocket(Network::Socket* socket, uint32_t filter,
                         void* user_data) {
	LOGGING_DEBUG("KqueueManager::AddSocket()");
	if (socket->GetFd() < 1) {
		Logging::Error("KqueueManager::AddSocket(): uninitialized socket passed");
		return false;
	}
	
	//create our kqueue event struct and add it to the kfd
	struct kevent event[1];
	memset(&event, 0, sizeof(struct kevent));
	PrivateData* data = new PrivateData();
	data->SetSocket(socket);
	data->SetUserData(user_data);

	EV_SET(&event[0], socket->GetFd(), LocalToSystem(filter), EV_ADD, 0, 0, data);
	int error;
	int ret = kevent(kfd, event, 1, NULL, 0, NULL);
	if (-1 == ret) {
		error = errno;
		Logging::Error("KqueueManager::AddSocket(): kevent(): %s", strerror(error));
		return false;
	}
	return true;
}

int16_t
KqueueManager::SystemToLocal(int16_t filter_in) {
	int16_t filter_out = 0;
	if (filter_in == EVFILT_READ) {
		LOGGING_DEBUG("KqueueManager::SystemToLocal(): ORing flag Readable");
		filter_out = Readable;
	}
	if (filter_in == EVFILT_WRITE) {
		LOGGING_DEBUG("KqueueManager::SystemToLocal(): ORing flag Writeable");
		filter_out = Writeable;
	}
	/*
	if (filter_in & EPOLLHUP) {
		LOGGING_DEBUG("KqueueManager::SystemToLocal(): ORing flag Hangup");
		filter_out |= Hangup;
	}
	if (filter_in & EPOLLERR) {
		LOGGING_DEBUG("KqueueManager::SystemToLocal(): ORing flag Error");
		filter_out |= Error;
	}
	*/
	LOGGING_DEBUG("KqueueManager::SystemToLocal(0x%x) = 0x%x",
	               filter_in, filter_out);
	return filter_out;
}

int16_t
KqueueManager::LocalToSystem(int16_t filter_in) {
	int16_t filter_out = 0;
	if (filter_in & Readable) {
		LOGGING_DEBUG("KqueueManager::LocalToSystem(): ORing flag EVFILT_READ");
		filter_out = EVFILT_READ;
	}
	if (filter_in & Writeable) {
		LOGGING_DEBUG("KqueueManager::LocalToSystem(): ORing flag EVFILT_WRITE");
		filter_out = EVFILT_WRITE;
	}
	/*
	if (filter_in & Hangup) {
		LOGGING_DEBUG("KqueueManager::LocalToSystem(): ORing flag EPOLLHUP");
		filter_out |= EPOLLHUP;
	}
	if (filter_in & Error) {
		LOGGING_DEBUG("KqueueManager::LocalToSystem(): ORing flag EPOLLERR");
		filter_out |= EPOLLERR;
	}
	*/
	LOGGING_DEBUG("KqueueManager::LocalToSystem(0x%x) = 0x%x",
	               filter_in, filter_out);
	return filter_out;
}

bool
KqueueManager::RemoveSocket(Network::Socket* socket) {
	if (socket == NULL) {
		LOGGING_DEBUG("KqueueManager::RemoveSocket(): socket was NULL!");
		return false;
	}
	struct kevent events[1];
	EV_SET(&events[0], socket->GetFd(), EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
	int error;
	int ret;
	ret = kevent(kfd, events, 1, NULL, 0, NULL);
	if (-1 == ret) {
		error = errno;
		/*
		Logging::Error("KqueueManager::RemoveSocket(EVFILT_WRITE): kevent(): %s",
		               strerror(error));
		*/
		// TODO: figure out how to deal with the fact that EV_DELETE requires
		// the exact filter it was added with
		//return false;
	}
	
	EV_SET(&events[0], socket->GetFd(), EVFILT_READ, EV_DELETE, 0, 0, NULL);
	ret = kevent(kfd, events, 1, NULL, 0, NULL);
	if (-1 == ret) {
		error = errno;
		/*
		Logging::Error("KqueueManager::RemoveSocket(EVFILT_READ): kevent(): %s",
		               strerror(error));
		*/
		// TODO: figure out how to deal with the fact that EV_DELETE requires
		// the exact filter it was added with
		//return false;
	}

	return true;
}

} //end namespace
