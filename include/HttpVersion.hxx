/*
 * HttpVersion
 * by Michael Spiegle
 * 10.23.09
 *
 * HttpVersion.hxx
 */

#ifndef _PUNISHER_HTTPVERSION_HXX_
#define _PUNISHER_HTTPVERSION_HXX_

#include "String.hxx"

namespace Punisher {

typedef enum {
	HTTP_10 = 10,
	HTTP_11 = 11
} http_version_t;

class HttpVersion {
	private:
		http_version_t version;

	public:
		HttpVersion();
		~HttpVersion();

		http_version_t Get() const;
		bool           Set(http_version_t version);
		bool           Set(const char* version);
		bool           Set(const M::String& version);
		M::String      ToString() const;
};

}

#endif
