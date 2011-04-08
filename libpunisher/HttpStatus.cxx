/*
 * HttpStatus
 * by Michael Spiegle
 * 11.22.09
 *
 * HttpStatus.cxx
 */

#include "HttpStatus.hxx"

#include <string>
#include <stdlib.h>

namespace Punisher {

HttpStatus::HttpStatus() {
	status = HTTP_NONE;
}

HttpStatus::~HttpStatus() {
}

http_status_t
HttpStatus::Get() const {
	return status;
}

bool
HttpStatus::Set(http_status_t status) {
	this->status = status;
	return true;
}

bool
HttpStatus::Set(const char* status) {
	return Set(atoi(status));
}

bool
HttpStatus::Set(const M::String& status) {
	return Set(status.ToCString());
}

bool
HttpStatus::Set(int status) {
	if (IS_VALID_HTTP_CODE(status)) {
		this->status = (http_status_t)status;
		return true;
	}
	return false;
}

M::String
HttpStatus::ToString() const {
	switch (status) {
		case HTTP_NONE:
			return "Invalid Status";

		case HTTP_CONTINUE:
			return "Continue";

		case HTTP_SWITCHING_PROTOCOLS:
			return "Switching Protocols";

		case HTTP_OK:
			return "Ok";

		case HTTP_CREATED:
			return "Created";

		case HTTP_ACCEPTED:
			return "Accepted";

		case HTTP_NON_AUTHORITATIVE:
			return "Non-Authoritative Information";

		case HTTP_NO_CONTENT:
			return "No Content";

		case HTTP_MOVED_PERMANENTLY:
			return "Moved Permanently";

		case HTTP_FOUND:
			return "Found";

		case HTTP_BAD_REQUEST:
			return "Bad Request";

		case HTTP_UNAUTHORIZED:
			return "Unauthorized";

		case HTTP_FORBIDDEN:
			return "Forbidden";

		case HTTP_NOT_FOUND:
			return "Not Found";

		case HTTP_METHOD_NOT_ALLOWED:
			return "Method Not Allowed";

		case HTTP_INTERNAL_SERVER_ERROR:
			return "Internal Server Error";

		case HTTP_NOT_IMPLEMENTED:
			return "Not Implemented";

		case HTTP_BAD_GATEWAY:
			return "Bad Gateway";

		case HTTP_SERVICE_UNAVAILABLE:
			return "Service Unavailable";

		default:
			return "Invalid Status";
	}
	return "Invalid Status";
}

}
