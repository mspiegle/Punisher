/*
 * Punisher
 * by Michael Spiegle
 * 09.04.09
 *
 * HttpRequest.h
 */

#ifndef _PUNISHER_HTTPREQUEST_H_
#define _PUNISHER_HTTPREQUEST_H_

#include "Request.h"
#include "HttpMethod.h"
#include "HttpVersion.h"
#include "HttpState.h"
#include "String.h"

#include <map>
#include <netinet/in.h>

namespace Punisher {

#define HEADER_MAP std::map<M::String, M::String>

typedef enum {
	HTTP_FIELD_MIN = 500,
	HTTP_FIELD_URL,
	HTTP_FIELD_METHOD,
	HTTP_FIELD_VERSION,
	HTTP_FIELD_MAX
} http_field_t;

class HttpRequest : public Request {
	private:
		M::String      uri;
		M::String      path;
		HttpVersion    version;
		HttpMethod     method;
		HEADER_MAP     headers;

	public:
		HttpRequest();
		~HttpRequest();

		const M::String& GetPath() const;
		void             SetPath(const M::String& path);

		const M::String& GetUri() const;
		bool             SetUri(const M::String& url);

		HttpMethod& Method();
		HttpVersion& Version();

		const HttpMethod& Method() const;
		const HttpVersion& Version() const;

		//const M::String& GetRequest() const;
		//bool Process();
		HttpState* CreateState() const;
		Network::Socket* CreateSocket() const;

		bool SetField(int field, const char* value);

		const HEADER_MAP& GetHeaders() const;
		void              SetHeaders(const HEADER_MAP& headers);

		M::String& GetHeader(const M::String& key);
		void       SetHeader(const M::String& key, const M::String& value);
};
#undef HEADER_MAP

}

#endif
