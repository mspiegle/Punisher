/*
 * Event
 * by Michael Spiegle
 * 09.16.09
 *
 * EpollManager.cpp
 */

#include "EpollManager.h"
#include "Logging.h"

#include <sys/epoll.h>
#include <unistd.h>

namespace Event {

EpollManager::EpollManager() {
	LOGGING_DEBUG("EpollManager::EpollManager()");
	efd = epoll_create(max_events);
}
EpollManager::~EpollManager() {
	LOGGING_DEBUG("EpollManager::~EpollManager()");
	close(efd);
}

int
EpollManager::Wait(int timeout) {
	LOGGING_DEBUG("EpollManager::Wait(int [%d])", timeout);
	int num_events = -1;
	if (-1 == (num_events = epoll_wait(efd, event_list, max_events, timeout))) {
		Logging::Error("EpollManager::Wait(): epoll_wait() error, backing off");
		sleep(1);
	}
	for (int x = 0; x < num_events; x++) {
		Item item;
		item.SetFilter(SystemToLocal(event_list[x].events));
		PrivateData* data = static_cast<PrivateData*>(event_list[x].data.ptr);
		item.SetUserData(data->GetUserData());
		item.SetSocket(data->GetSocket());
		delete(data);
		events.push(item);
	}
	return num_events;
}

bool
EpollManager::AddSocket(Network::Socket* socket, uint32_t filter,
                        void* user_data) {
	LOGGING_DEBUG("EpollManager::AddSocket()");
	if (socket->GetFd() < 1) {
		return false;
	}
	
	//create our epoll event struct and add it to the efd
	epoll_event event;
	PrivateData* data = new PrivateData();
	data->SetSocket(socket);
	data->SetUserData(user_data);
	event.data.ptr = data;
	event.events = LocalToSystem(filter);
	int ret = epoll_ctl(efd, EPOLL_CTL_ADD, data->GetSocket()->GetFd(), &event);
	return (ret == 0) ? true : false;
}

uint32_t
EpollManager::SystemToLocal(uint32_t filter_in) {
	uint32_t filter_out = 0;
	if (filter_in & EPOLLIN) {
		LOGGING_DEBUG("EpollManager::SystemToLocal(): ORing flag Readable");
		filter_out |= Readable;
	}
	if (filter_in & EPOLLOUT) {
		LOGGING_DEBUG("EpollManager::SystemToLocal(): ORing flag Writeable");
		filter_out |= Writeable;
	}
	if (filter_in & EPOLLHUP) {
		LOGGING_DEBUG("EpollManager::SystemToLocal(): ORing flag Hangup");
		filter_out |= Hangup;
	}
	if (filter_in & EPOLLERR) {
		LOGGING_DEBUG("EpollManager::SystemToLocal(): ORing flag Error");
		filter_out |= Error;
	}
	LOGGING_DEBUG("EpollManager::SystemToLocal(0x%x) = 0x%x",
	               filter_in, filter_out);
	return filter_out;
}

uint32_t
EpollManager::LocalToSystem(uint32_t filter_in) {
	uint32_t filter_out = 0;
	if (filter_in & Readable) {
		LOGGING_DEBUG("EpollManager::LocalToSystem(): ORing flag EPOLLIN");
		filter_out |= EPOLLIN;
	}
	if (filter_in & Writeable) {
		LOGGING_DEBUG("EpollManager::LocalToSystem(): ORing flag EPOLLOUT");
		filter_out |= EPOLLOUT;
	}
	if (filter_in & Hangup) {
		LOGGING_DEBUG("EpollManager::LocalToSystem(): ORing flag EPOLLHUP");
		filter_out |= EPOLLHUP;
	}
	if (filter_in & Error) {
		LOGGING_DEBUG("EpollManager::LocalToSystem(): ORing flag EPOLLERR");
		filter_out |= EPOLLERR;
	}
	LOGGING_DEBUG("EpollManager::LocalToSystem(0x%x) = 0x%x",
	               filter_in, filter_out);
	return filter_out;
}

bool
EpollManager::RemoveSocket(Network::Socket* socket) {
	if (socket == NULL) {
		return false;
	}
	int ret = epoll_ctl(efd, EPOLL_CTL_DEL, socket->GetFd(), NULL);
	return (ret == 0) ? true : false;
}

}
