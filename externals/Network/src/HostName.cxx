/*
 * HostName
 * by Michael Spiegle
 * 10.28.09
 *
 * HostName.cxx
 */

#include "HostName.hxx"
#include "Logging.hxx"

#include <string>

namespace Network {

HostName::HostName() {
	LOGGING_DEBUG("HostName::HostName()");
}

HostName::~HostName() {
	LOGGING_DEBUG("HostName::~HostName()");
}

bool
HostName::Set(const M::String& name) {
	LOGGING_DEBUG("HostName::Set(M::String&)");
	this->name = name;
	return true;
}

bool
HostName::Set(const char* name) {
	LOGGING_DEBUG("HostName::Set(const char*)");
	this->name = name;
	return true;
}

const M::String&
HostName::ToString() const {
	return name;
}

}
