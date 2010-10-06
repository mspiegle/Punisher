/*
 * Host
 * by Michael Spiegle
 * 10.28.09
 *
 * Host.cpp
 */

#include "Host.h"
#include "Logging.h"
#include "Resolve.h"
#include "String.h"

#include <arpa/inet.h>

namespace Network {

Host::Host() {
	LOGGING_DEBUG("Host::Host()");
}

Host::~Host() {
	LOGGING_DEBUG("Host::~Host()");
}

const HostAddress&
Host::Address() const {
	return address;
}

const HostPort&
Host::Port() const {
	return port;
}

const HostName&
Host::Name() const {
	return name;
}

bool
Host::SetName(const char* name) {
	LOGGING_DEBUG("Host::SetName(const char* [%s])", name);
	if (this->name.Set(name)) {
		//we should also resolve this name to an IP.
		//if an IP was passed in for the hostname, then the net result is that
		//both the hostname and address should be set to the IP
		struct in_addr address;
		if (Resolve::NameToAddress(name, &address)) {
			this->address.Set(address);
			return true;
		}
	}
	LOGGING_DEBUG("Host::SetName(): Unable to set name");
	return false;
}

bool
Host::SetName(const M::String& name) {
	LOGGING_DEBUG("Host::SetName(M::String& [%s])", name.ToCString());
	return SetName(name.ToCString());
}

bool
Host::SetAddress(const char* address) {
	LOGGING_DEBUG("Host::SetAddress(char* [%s])", address);
	return SetName(address);
}

bool
Host::SetAddress(const M::String& address) {
	LOGGING_DEBUG("Host::SetAddress(M::String [%s])", address.ToCString());
	return SetAddress(address.ToCString());
}

bool
Host::SetAddress(const in_addr& address) {
	LOGGING_DEBUG("Host::SetAddress(in_addr [%d])", address);
	char buffer[16];
	if (NULL == inet_ntop(AF_INET, &address, buffer, sizeof(buffer))) {
		return false;
	}
	return SetAddress(buffer);
}

bool
Host::SetPort(uint16_t port) {
	LOGGING_DEBUG("Host::SetPort(uint16_t [%u])", port);
	return this->port.Set(port);
}

M::String
Host::ToString() const {
	M::String temp;
	temp += Name().ToString().ToCString();
	temp += ":";
	temp += Port().ToString().ToCString();
	return temp;
}

}
