/*
 * Punisher
 * by Michael Spiegle
 * 09.03.09
 *
 * Request.cxx
 */

#include "Request.hxx"

namespace Punisher {

Request::Request() {
	validator = NULL;
	processed = false;
}

Request::~Request() {
}

Validator*&
Request::GetValidator() const {
	return validator;
}

Network::Host&
Request::GetHost() {
	return host;
}

const Network::Host&
Request::GetHost() const {
	return host;
}

}
