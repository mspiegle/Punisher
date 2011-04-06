/*
 * Punisher
 * by Michael Spiegle
 * 10.19.09
 *
 * HttpProtocol.cxx
 */

#include "HttpProtocol.hxx"
#include "Logging.hxx"
#include "HttpRequest.hxx"
#include "Socket.hxx"
#include "TcpSocket.hxx"
#include "Network.hxx"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

namespace Punisher {

void
HttpProtocol::Init() {
	state = SEND_REQUEST;
	proto_content_length = 0;
	parsed_content_length = 0;
	iter = 0;
	keepalive = false;
}

HttpProtocol::HttpProtocol() {
	Init();
}

HttpProtocol::HttpProtocol(const HttpRequest* request) {
	LOGGING_DEBUG("HttpProtocol::HttpProtocol()");
	Init();

	//TODO: Figure out why I need a cast, and can't use a safe cast
	this->request = (const Request*)request;

	//if this is an HTTP/1.1 request, we can try using keepalive
	if (request->Version().Get() == HTTP_11) {
		LOGGING_DEBUG("HttpProtocol::HttpProtocol(): enable HTTP/1.1 keepalive");
		keepalive = true;
	}
}

HttpProtocol::~HttpProtocol() {
}

protocol_result_t
HttpProtocol::ReadData(Network::Socket* socket) {
	LOGGING_DEBUG("HttpProtocol::ReadData()");

	unsigned char buffer[4096];
	size_t buffer_size = sizeof(buffer);
	protocol_result_t ret;
	switch (socket->Read(buffer, &buffer_size)) {
		case Network::NETWORK_SUCCESS:
			break;

		case Network::NETWORK_ERROR:
			Logging::Info("HttpProtocol::ReadData(): socket read failed");
			ret.success = false;
			return ret;
			break;

		case Network::NETWORK_WOULDBLOCK:
			ret.success = true;
			ret.state = STATE_READ;
			ret.bytes_transferred = 0;
			return ret;
			break;

		default:
			//TODO: shouldn't we return from here?
			Logging::Error("HttpProtocol::ReadData(): Bad return from Socket::Read()");
			break;
	}

	ret.bytes_transferred = buffer_size;

	for (unsigned int x = 0; x < buffer_size; ++x) {

		//ignore '\r'
		if (buffer[x] == '\r' && state != NEED_BODY) {
			continue;
		}

		//make sure we don't exceed our local buffer
		if (iter >= sizeof(buffer)) {
			//exceeding our buffer usually means there was a protocol error
			Logging::Error("HttpProtocol::ReadData(): Error reading protocol");
			state = HTTP_DONE;
			error = "Protocol Error: URL[";
			error += ((HttpRequest*)request)->GetPath();
			error += "]";
			ret.success = false;
			return ret;
		}

		switch (state) {
			case NEED_PROTO:
				if (buffer[x] == '/') {
					//verify that the protocol is HTTP
					if (true == fast_strcmp4(temp, 'H', 'T', 'T', 'P')) {
						state = NEED_VERSION;
						iter = 0;
					} else {
						Logging::Error("HttpProtocol::ReadData(): Incorrect protocol");
						state = HTTP_DONE;
						error = "Protocol is not HTTP";
						ret.success = false;
						return ret;
					}
				} else {
					temp[iter] = buffer[x];
					++iter;
				}
				break;

			case NEED_VERSION:
				if (buffer[x] == ' ') {
					temp[iter] = '\0';
					if (version.Set(temp)) {
						state = NEED_STATUS_CODE;
						iter = 0;
					} else {
						Logging::Error("HttpProtocol::ReadData(): Invalid version");
						state = HTTP_DONE;
						error = "Unknown HTTP version";
						ret.success = false;
						return ret;
					}
				} else {
					temp[iter] = buffer[x];
					++iter;
				}
				break;

			case NEED_STATUS_CODE:
				if (buffer[x] == ' ') {
					temp[iter] = '\0';
					if (status.Set(temp)) {
						state = NEED_STATUS_TXT;
						iter = 0;
					} else {
						Logging::Error("HttpProtocol::ReadData(): Invalid status code");
						state = HTTP_DONE;
						error = "Unknown HTTP status code";
						ret.success = false;
						return ret;
					}
				} else {
					temp[iter] = buffer[x];
					++iter;
				}
				break;

			case NEED_STATUS_TXT:
				if (buffer[x] == '\n') {
					//we don't really need the status text because we already
					//have the status code and it's a much more reliable/deterministic
					//way to get the result
					state = NEED_HKEY;
				}
				break;

			case NEED_HKEY:
				if (buffer[x] == ':') {
					temp[iter] = '\0';
					header_key = temp;
					state = NEED_HVAL;
					iter = 0;
				} else {
					temp[iter] = buffer[x];
					++iter;
				}
				break;

			case NEED_HVAL:
				if (buffer[x] == '\n') {
					//all we really care about is the content-length for now...
					temp[iter] = '\0';
					if (0 == strncmp("Content-Length", header_key.ToCString(), 15)) {
						proto_content_length = strtoll(temp, NULL, 10);
					}
					if (0 == strncmp("Connection", header_key.ToCString(), 10)) {
						if (0 == strncmp("close", temp, 5)) {
							LOGGING_DEBUG("HttpProtocol::ReadData(): keepalive disabled");
							keepalive = false;
						}
					}
					state = NEED_HFIN;
					iter = 0;
				} else {
					//we want to skip the first character IF it's a space
					if (iter == 0 && buffer[x] == ' ') {
						continue;
					}
					temp[iter] = buffer[x];
					++iter;
				}
				break;

			case NEED_HFIN:
				//this tells us if there's another header to read, or if we should
				//continue onto the next state
				if (buffer[x] == '\n') {
					state = HEADER_CHECK;
				} else {
					state = NEED_HKEY;
					--x;
				}
				break;

			case HEADER_CHECK:
				//this gives us a chance to verify that we got the necessary headers
				//currently, we're only checking for content_length
				if (proto_content_length < 1) {
					LOGGING_DEBUG("HttpProtocol::ReadData(): Missing Content-Length");
					state = HTTP_DONE;
					error = "No Content-Length header provided";
					ret.success = false;
					return ret;
				}
				state = NEED_BODY;
				--x;
				break;

			case NEED_BODY:
				//catch our local buffer before it overflows
				if (iter >= (sizeof(buffer) - 1)) {
					//call out for validation and reset buffer
					if (NULL != validator) {
						validator->Update(temp, iter);
					}
					iter = 0;
				}

				//update buffer and keep track of how many bytes we've read on the body
				temp[iter] = buffer[x];
				++iter;
				++parsed_content_length;

				if (parsed_content_length >= proto_content_length) {
					//we're done? make final validation call and return
					if (NULL != validator) {
						validator->Update(temp, iter);
						validator->Final();
						if (true != validator->Compare(request->GetValidator())) {
							state = HTTP_DONE;
							error = "Validation failure: URL[";
							error += ((HttpRequest*)request)->GetPath();
							error += "] WANT[";
							error += request->GetValidator()->ToString();
							error += "] GOT[";
							error += validator->ToString();
							error += "]";
							ret.success = false;
							return ret;
						}
					}
					state = HTTP_DONE;
					ret.success = true;
					ret.state = STATE_DONE;
					ret.keepalive = keepalive;
					return ret;
				}

				break;

			case HTTP_DONE:
				ret.success = true;
				ret.state = STATE_DONE;
				ret.keepalive = keepalive;
				return ret;
				break;

			default:
				Logging::Error("HttpProtocol::ReadData(): Invalid http state");
				state = HTTP_DONE;
				error = "Unknown error";
				ret.success = false;
				return ret;
				break;
		}
	}
	ret.success = true;
	ret.state = STATE_READ;
	return ret;
}

protocol_result_t
HttpProtocol::WriteData(Network::Socket* socket) {
	size_t data_len = 0;
	protocol_result_t ret;

	if (SEND_REQUEST == state) {
		M::String buffer;
		buffer += GetRequest()->Method().ToString();
		buffer += " ";
		buffer += GetRequest()->GetPath();
		buffer += " ";
		buffer += "HTTP/";
		buffer += GetRequest()->Version().ToString();
		buffer += "\r\n";
		buffer += "Host: ";
		buffer += GetRequest()->GetHost().Name().ToString();
		//buffer += "\r\n";
		//buffer += "Connection: close";
		buffer += "\r\n";
		buffer += "\r\n";

		const char* data = buffer.ToCString();
		data_len = buffer.Length();

		if (Network::NETWORK_SUCCESS == socket->Write(data, &data_len)) {
			state = NEED_PROTO;
			ret.success = true;
			ret.state = STATE_READ;
			ret.bytes_transferred = data_len;
			return ret;
		}
	}
	error = "Error sending request";
	ret.success = false;
	return ret;
}

Network::network_error_t
HttpProtocol::Connect(Network::Socket* socket) {
	if (NULL != socket) {
		const Network::Host& host = request->GetHost();
		return static_cast<Network::TcpSocket*>(socket)->Connect(host);
	}
	return Network::NETWORK_ERROR;
}

const HttpRequest*
HttpProtocol::GetRequest() const {
	return static_cast<const HttpRequest*>(this->request);
}

}
