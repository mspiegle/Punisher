/*
 * HttpMethod
 * by Michael Spiegle
 * 10.23.09
 *
 * HttpMethod.cxx
 */

#include "HttpMethod.hxx"
#include "String.hxx"

#include <string.h>

namespace Punisher {

HttpMethod::HttpMethod() {
	method = HTTP_GET;
}

HttpMethod::~HttpMethod() {
}

http_method_t
HttpMethod::Get() const {
	return method;
}

bool
HttpMethod::Set(http_method_t method) {
	this->method = method;
	return true;
}

bool
HttpMethod::Set(const char* method) {
	if (0 == strncmp(method, "GET", 4)) {
		return Set(HTTP_GET);
	} else if (0 == strncmp(method, "PUT", 4)) {
		return Set(HTTP_PUT);
	} else if (0 == strncmp(method, "POST", 5)) {
		return Set(HTTP_POST);
	} else if (0 == strncmp(method, "DELETE", 7)) {
		return Set(HTTP_DELETE);
	} else {
		return Set(HTTP_GET);
	}
	return false;
}

bool
HttpMethod::Set(const M::String& method) {
	return Set(method.ToCString());
}

M::String
HttpMethod::ToString() const {
	switch (method) {
		case HTTP_GET:
			return "GET";
			break;

		case HTTP_PUT:
			return "PUT";
			break;

		case HTTP_POST:
			return "POST";
			break;

		case HTTP_DELETE:
			return "DELETE";
			break;
	}
	return "GET";
}

}
