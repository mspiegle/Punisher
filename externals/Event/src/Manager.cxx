/*
 * Event
 * by Michael Spiegle
 * 09.16.09
 *
 * Manager.cxx
 */

#include "Manager.hxx"
#include "Logging.hxx"

namespace Event {

Manager::Manager() {
	LOGGING_DEBUG("Manager::Manager()");
}

Manager::~Manager() {
	LOGGING_DEBUG("Manager::~Manager()");
}

Item
Manager::NextEvent() {
	Item item = events.front();
	events.pop();
	return item;
}

PrivateData::PrivateData() {
	LOGGING_DEBUG("PrivateData::PrivateData()");
}
PrivateData::~PrivateData() {
	LOGGING_DEBUG("PrivateData::~PrivateData()");
}

void*
PrivateData::GetUserData() const {
	return user_data;
}

void
PrivateData::SetUserData(void* user_data) {
	this->user_data = user_data;
}

Network::Socket*
PrivateData::GetSocket() const {
	return socket;
}

void
PrivateData::SetSocket(Network::Socket* socket) {
	this->socket = socket;
}

}
