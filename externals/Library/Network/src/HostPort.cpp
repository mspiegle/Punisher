/*
 * HostPort
 * by Michael Spiegle
 * 10.28.09
 *
 * HostPort.cpp
 */

#include "HostPort.h"
#include "Logging.h"

#include <string>
#include <stdint.h>
#include <stdio.h>
#include <arpa/inet.h>

namespace Network {

HostPort::HostPort() {
	LOGGING_DEBUG("HostPort::HostPort()");
	port = 0;
}

HostPort::~HostPort() {
	LOGGING_DEBUG("HostPort::~HostPort()");
}

bool
HostPort::Set(uint16_t port) {
	this->port = port;
	return true;
}

uint16_t
HostPort::Get() const {
	return port;
}

M::String
HostPort::ToString() const {
	char buffer[9];
	snprintf(buffer, sizeof(buffer), "%d", port);

	return M::String(buffer);
}

uint16_t
HostPort::ToNetwork() const {
	return htons(port);
}

}
