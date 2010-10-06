/*
 * HttpMethod
 * by Michael Spiegle
 * 10.23.09
 *
 * HttpMethod.h
 */

#ifndef _PUNISHER_HTTPMETHOD_H_
#define _PUNISHER_HTTPMETHOD_H_

#include "String.h"

namespace Punisher {

typedef enum {
	HTTP_GET = 300,
	HTTP_PUT,
	HTTP_POST,
	HTTP_DELETE
} http_method_t;

class HttpMethod {
	private:
		http_method_t method;

	public:
		HttpMethod();
		~HttpMethod();

		http_method_t Get() const;
		bool          Set(http_method_t method);
		bool          Set(const char* method);
		bool          Set(const M::String& method);
		M::String     ToString() const;
};



}

#endif
