/*
 * HostAddress
 * by Michael Spiegle
 * 10.28.09
 *
 * HostAddress.cxx
 */

#include "HostAddress.hxx"
#include "Logging.hxx"

#include <string>
#include <string.h>
#include <arpa/inet.h>

namespace Network {

HostAddress::HostAddress() {
	LOGGING_DEBUG("HostAddress::HostAddress()");
	memset(&address, 0, sizeof(address));
}

HostAddress::~HostAddress() {
	LOGGING_DEBUG("HostAddress::~HostAddress()");
}

bool
HostAddress::Set(const struct in_addr& address) {
	memcpy(&(this->address), &address, sizeof(in_addr));
	return true;
}

bool
HostAddress::Set(const char* address) {
	if (0 == inet_aton(address, &(this->address))) {
		return false;
	}
	return true;
}

const struct in_addr&
HostAddress::Get() const {
	return address;
}

M::String
HostAddress::ToString() const {
	char buffer[16];
	if (NULL == inet_ntop(AF_INET, &address, buffer, sizeof(buffer))) {
		return M::String();
	}

	return M::String(buffer);
}

}
