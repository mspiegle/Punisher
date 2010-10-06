/*
 * Punisher
 * by Michael Spiegle
 * 10.19.09
 *
 * HttpState.h
 */

#ifndef _PUNISHER_HTTPSTATE_H_
#define _PUNISHER_HTTPSTATE_H_

#include "State.h"
#include "HttpStatus.h"
#include "HttpVersion.h"
#include "String.h"
#include "Network.h"
#include "Socket.h"

#include <map>
#include <stdint.h>

namespace Punisher {

#define fast_strcmp4(x, a, b, c, d) \
                     ((x[0] == a) && (x[1] == b) && (x[2] == c) && (x[3] == d))

class HttpRequest;

typedef enum {
	SEND_REQUEST = 0,
	NEED_PROTO,
	NEED_VERSION,
	NEED_STATUS_CODE,
	NEED_STATUS_TXT,
	NEED_HKEY,
	NEED_HVAL,
	NEED_HFIN,
	HEADER_CHECK,
	NEED_BODY,
	HTTP_DONE
} http_state_t;

static const size_t buffer_size = 4096;

#define HEADER_MAP std::map<M::String, M::String>

class HttpState : public State {
	private:
		http_state_t state;
		HEADER_MAP   header_map;

		HttpVersion  version;
		HttpStatus   status;
		size_t       proto_content_length;
		size_t       parsed_content_length;
		char         temp[buffer_size];
		size_t       iter;
		M::String    header_key;
		bool         keepalive;

	public:
		HttpState();
		HttpState(const HttpRequest* request);
		~HttpState();
		void Init();

		state_result_t ReadData(Network::Socket* socket);
		state_result_t WriteData(Network::Socket* socket);

		Network::network_error_t Connect(Network::Socket* socket);

		const HttpRequest* GetRequest() const;
};

#undef HEADER_MAP

}

#endif
