/*
 * Punisher
 * by Michael Spiegle
 * 10.15.09
 *
 * State.cpp
 */

#include "State.h"

namespace Punisher {

State::State() {
	request = NULL;
	validator = NULL;
}

State::State(const Request* request) {
	this->request = request;
}

State::~State() {
	if (NULL != validator) {
		delete(validator);
		validator = NULL;
	}
}

const Request*
State::GetRequest() const {
	return request;
}

Validator*&
State::GetValidator() const {
	return validator;
}

}
