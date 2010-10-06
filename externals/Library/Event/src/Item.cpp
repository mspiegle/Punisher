/*
 * Event
 * by Michael Spiegle
 * 09.16.09
 *
 * Item.cpp
 */

#include "Item.h"
#include "Logging.h"

namespace Event {

Item::Item() {
	LOGGING_DEBUG("Item::Item()");
	socket = NULL;
	user_data = NULL;
}
Item::~Item() {
	LOGGING_DEBUG("Item::~Item()");
}

bool
Item::IsWriteable() const {
	return (filter & Event::Writeable);
}

bool
Item::IsReadable() const {
	return ((filter & Event::Readable) && !(socket->Listening()));
}

bool
Item::IsAcceptable() const {
	return ((filter & Event::Readable) && socket->Listening());
}

bool
Item::IsHangup() const {
	return (filter & Event::Hangup);
}

bool
Item::IsError() const {
	return (filter & Event::Error);
}

void
Item::SetFilter(uint32_t filter) {
	this->filter = filter;
}

void*
Item::GetUserData() const {
	return user_data;
}

void
Item::SetUserData(void* user_data) {
	this->user_data = user_data;
}

Network::Socket*
Item::GetSocket() const {
	return socket;
}

void
Item::SetSocket(Network::Socket* socket) {
	this->socket = socket;
}

}
