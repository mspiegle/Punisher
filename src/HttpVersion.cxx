/*
 * HttpVersion
 * by Michael Spiegle
 * 10.23.09
 *
 * HttpVersion.cxx
 */

#include "HttpVersion.hxx"
#include "String.hxx"

#include <string.h>

namespace Punisher {

HttpVersion::HttpVersion() {
	version = HTTP_10;
}

HttpVersion::~HttpVersion() {
}

http_version_t
HttpVersion::Get() const {
	return version;
}

bool
HttpVersion::Set(http_version_t version) {
	this->version = version;
	return true;
}

bool
HttpVersion::Set(const char* version) {
	if (0 == strncmp(version, "1.0", 3)) {
		return Set(HTTP_10);
	} else if (0 == strncmp(version, "1.1", 3)) {
		return Set(HTTP_11);
	} else if (0 == strncmp(version, "10", 2)) {
		return Set(HTTP_10);
	} else if (0 == strncmp(version, "11", 2)) {
		return Set(HTTP_11);
	} else {
		return Set(HTTP_10);
	}
	return false;
}

bool
HttpVersion::Set(const M::String& version) {
	return Set(version.ToCString());
}

M::String
HttpVersion::ToString() const {
	switch (version) {
		case HTTP_10:
			return "1.0";
			break;

		case HTTP_11:
			return "1.1";
			break;
	}
	return "1.0";
}

}
