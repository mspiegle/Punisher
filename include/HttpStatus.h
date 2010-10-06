/*
 * HttpStatus
 * by Michael Spiegle
 * 11.22.09
 *
 * HttpStatus.h
 */

#ifndef _PUNISHER_HTTPSTATUS_H_
#define _PUNISHER_HTTPSTATUS_H_

#include "String.h"

namespace Punisher {

//built from RFC2616 section 10
typedef enum {
	HTTP_NONE = 0,
	HTTP_CONTINUE = 100,
	HTTP_SWITCHING_PROTOCOLS = 101,
	HTTP_OK = 200,
	HTTP_CREATED = 201,
	HTTP_ACCEPTED = 202,
	HTTP_NON_AUTHORITATIVE = 203,
	HTTP_NO_CONTENT = 204,
	HTTP_RESET_CONTENT = 205,
	HTTP_PARTIAL_CONTENT = 206,
	HTTP_MULTIPLE_CHOICES = 300,
	HTTP_MOVED_PERMANENTLY = 301,
	HTTP_FOUND = 302,
	HTTP_SEE_OTHER = 303,
	HTTP_NOT_MODIFIED = 304,
	HTTP_USE_PROXY = 305,
	HTTP_306 = 306,
	HTTP_TEMPORARY_REDIRECT = 307,
	HTTP_BAD_REQUEST = 400,
	HTTP_UNAUTHORIZED = 401,
	HTTP_PAYMENT_REQUIRED = 402,
	HTTP_FORBIDDEN = 403,
	HTTP_NOT_FOUND = 404,
	HTTP_METHOD_NOT_ALLOWED = 405,
	HTTP_NOT_ACCEPTABLE = 406,
	HTTP_PROXY_AUTH_REQUIRED = 407,
	HTTP_REQUEST_TIMEOUT = 408,
	HTTP_CONFLICT = 409,
	HTTP_GONE = 410,
	HTTP_LENGTH_REQUIRED = 411,
	HTTP_PRECONDITION_FAILED = 412,
	HTTP_REQUEST_ENTITY_TOO_LARGE = 413,
	HTTP_REQUEST_URI_TOO_LONG = 414,
	HTTP_UNSUPPORTED_MEDIA_TYPE = 415,
	HTTP_REQUESTED_RANGE_NOT_SATISFIABLE = 416,
	HTTP_EXPECTATION_FAILED = 417,
	HTTP_INTERNAL_SERVER_ERROR = 500,
	HTTP_NOT_IMPLEMENTED = 501,
	HTTP_BAD_GATEWAY = 502,
	HTTP_SERVICE_UNAVAILABLE = 503,
	HTTP_GATEWAY_TIMEOUT = 504,
	HTTP_VERSION_NOT_SUPPORTED = 505
} http_status_t;

#define IS_HTTP_INFORMATIONAL(x) (x >= 100 && x <= 101)
#define IS_HTTP_SUCCESSFUL(x) (x >= 200 && x <= 206)
#define IS_HTTP_REDIRECTION(x) (x >= 300 && x <= 307)
#define IS_HTTP_CLIENT_ERROR(x) (x >= 400 && x <= 417)
#define IS_HTTP_SERVER_ERROR(x) (x >= 500 && x <= 505)
#define IS_VALID_HTTP_CODE(x) (IS_HTTP_INFORMATIONAL(x) || \
                               IS_HTTP_SUCCESSFUL(x) ||    \
                               IS_HTTP_REDIRECTION(x) ||   \
                               IS_HTTP_CLIENT_ERROR(x) ||  \
                               IS_HTTP_SERVER_ERROR(x))

class HttpStatus {
	private:
		http_status_t status;

	public:
		HttpStatus();
		~HttpStatus();

		http_status_t Get() const;
		bool          Set(http_status_t status);
		bool          Set(int status);
		bool          Set(const char* status);
		bool          Set(const M::String& status);
		M::String       ToString() const;
};

}

#endif
