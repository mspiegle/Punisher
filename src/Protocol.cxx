/*
 * Punisher
 * by Michael Spiegle
 * 10.15.09
 *
 * Protocol.cxx
 */

#include "Protocol.hxx"

#include <string.h>

namespace Punisher {

Protocol::Protocol() {
	request = NULL;
	validator = NULL;

	memset(&start_time, 0, sizeof(struct timeval));
	memset(&end_time, 0, sizeof(struct timeval));
}

Protocol::Protocol(const Request* request) {
	this->request = request;
}

Protocol::~Protocol() {
	if (NULL != validator) {
		delete(validator);
		validator = NULL;
	}
}

const Request*
Protocol::GetRequest() const {
	return request;
}

Validator*
Protocol::GetValidator() const {
	return validator;
}

void
Protocol::SetValidator(Validator* validator) {
	if (NULL != validator) {
		this->validator = validator;
	}
}

uint64_t
Protocol::GetRequestDuration() const {
	// find the difference between start_time and end_time in microseconds
	uint64_t start_usec = (start_time.tv_sec * 1000000) + start_time.tv_usec;
	uint64_t end_usec = (end_time.tv_sec * 1000000) + end_time.tv_usec;
	return (end_usec - start_usec);
}

}
