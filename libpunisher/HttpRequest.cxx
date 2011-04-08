/*
 * Punisher
 * by Michael Spiegle
 * 09.04.09
 *
 * HttpRequest.cxx
 */

#include "HttpRequest.hxx"
#include "HttpProtocol.hxx"
#include "Logging.hxx"
#include "TcpSocket.hxx"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

namespace Punisher {

HttpRequest::HttpRequest() {
}

HttpRequest::~HttpRequest() {
}

const M::String&
HttpRequest::GetPath() const {
	return path;
}
void
HttpRequest::SetPath(const M::String& path) {
	LOGGING_DEBUG("HttpRequest::SetPath(M::String [%s])", path.ToCString());
	this->path = path;
}

const M::String&
HttpRequest::GetUri() const {
	return uri;
}
bool
HttpRequest::SetUri(const M::String& uri) {
	this->uri = uri;
	M::String protocol, hostname, path;
	int port;

	//determine protocol - we only support http at the moment
	size_t protoPos;
	if (M::String::npos == (protoPos = uri.Find("://", 0))) {
		LOGGING_DEBUG("Protocol not found");
		return false;
	}
	protocol = uri.Substring(0, protoPos);
	if (0 != protocol.Compare("http")) {
		LOGGING_DEBUG("Protocol is not HTTP");
		return false;
	}

	//determine hostname - use protoPos+3 to start
	size_t hostPos;
	if (M::String::npos == (hostPos = uri.FindFirst(":/", protoPos + 3UL))) {
		LOGGING_DEBUG("Couldn't find start of hostname");
		return false;
	}
	hostname = uri.Substring(protoPos + 3, hostPos - protoPos - 3);

	//determine port if it exists
	size_t portPos, pathPos;
	if (M::String::npos == (portPos = uri.Find(":", protocol.Length() + 3 +
	    hostname.Length()))) {
		port = 80;
		pathPos = uri.Find("/", protocol.Length() + 3 + hostname.Length());
	} else {
		if (M::String::npos == (pathPos = uri.Find("/", portPos))) {
			port = atoi(uri.Substring(portPos + 1).ToCString());
		} else {
			port = atoi(uri.Substring(portPos + 1, pathPos - portPos - 1).ToCString());
		}
	}

	//determine path if exists
	if (M::String::npos == pathPos) {
		path = "/";
	} else {
		path = uri.Substring(pathPos);
	}

	if (!host.SetName(hostname)) {
		LOGGING_DEBUG("Invalid hostname");
		return false;
	}
	if (!host.SetPort(port)) {
		LOGGING_DEBUG("Invalid port");
		return false;
	}
	SetPath(path);

	return true;
}

const std::map<M::String, M::String>&
HttpRequest::GetHeaders() const {
	return headers;
}
void
HttpRequest::SetHeaders(const std::map<M::String, M::String>& headers) {
	this->headers = headers;
}

M::String&
HttpRequest::GetHeader(const M::String& key) {
	return headers[key];
}
void
HttpRequest::SetHeader(const M::String& key, const M::String& value) {
	headers[key] = value;
}

HttpMethod&
HttpRequest::Method() {
	return method;
}

HttpVersion&
HttpRequest::Version() {
	return version;
}

const HttpMethod&
HttpRequest::Method() const {
	return method;
}

const HttpVersion&
HttpRequest::Version() const {
	return version;
}

bool
HttpRequest::SetField(int field, const char* value) {
	if (field > HTTP_FIELD_MAX || field < HTTP_FIELD_MIN) {
		return false;
	}
	switch (field) {
		case HTTP_FIELD_URL:
			return SetUri(value);
			break;

		case HTTP_FIELD_METHOD:
			return Method().Set(value);
			break;

		case HTTP_FIELD_VERSION:
			return Version().Set(value);
			break;
	}
	return false;
}

/*
bool
HttpRequest::Process() {
	request = "";
	request += Method().ToString();
	request += " ";
	request += path;
	request += " ";
	request += "HTTP/";
	request += Version().ToString();
	request += "\r\n";
	request += "Host: ";
	request += GetHost().Name().ToString();
	request += "\r\n";
	request += "\r\n";

	processed = true;
	return true;
}
*/

/*
const M::String&
HttpRequest::GetRequest() const {
	return request;
}
*/

HttpProtocol*
HttpRequest::CreateProtocol() const {
	return new HttpProtocol(this);
}

Network::Socket*
HttpRequest::CreateSocket() const {
	return new Network::TcpSocket();
}

}
