/*
 * Punisher
 * by Michael Spiegle
 * 10.15.09
 *
 * Protocol.cxx
 */

#include "Protocol.hxx"

namespace Punisher {

Protocol::Protocol() {
	request = NULL;
	validator = NULL;
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

}
