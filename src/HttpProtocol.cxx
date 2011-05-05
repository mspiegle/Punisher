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
#include <errno.h>
#include <assert.h>

#define __STDC_FORMAT_MACROS
#include <inttypes.h>

namespace Punisher {

void
HttpProtocol::Init() {
	state = SEND_REQUEST;
	proto_content_length = 0;
	parsed_content_length = 0;
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
			// whoops - without this, everything is an error!
			break;

		case Network::NETWORK_ERROR:
			this->error = "HttpProtocol::ReadData(): socket read failed";
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
			// this shouldn't happen, but we'll definitely know if it does
			Logging::Error("HttpProtocol::ReadData(): Invalid network_error_t");
			assert(false);
			break;
	}

	// TODO: see if we can factor this out into the Statistics class
	ret.bytes_transferred = buffer_size;

	// handle connection finished situation
	bool finished = false;
	if (buffer_size == 0) {
		finished = true;
	}

	for (unsigned int x = 0; x <= buffer_size; ++x) {

		// ignore '\r', but not if we're parsing the body
		if ((buffer[x] == '\r') && (state != NEED_BODY)) {
			continue;
		}

		// make sure we don't exceed our local buffer
		if (temp.Length() >= sizeof(buffer)) {
			// exceeding our buffer usually means there was a protocol error
			Logging::Error("HttpProtocol::ReadData(): Error reading protocol");
			state = HTTP_DONE;
			this->error = "Protocol Error: URL[";
			this->error += ((HttpRequest*)request)->GetPath();
			this->error += "]";
			ret.success = false;
			return ret;
		}

		// this is the big-ass switch parser.  not pretty, but probably fast.
		switch (state) {
			case NEED_PROTO:
				if (buffer[x] == '/') {
					// verify that the protocol is HTTP
					if (true == temp.FastCompare4('H', 'T', 'T', 'P')) {
						temp.Erase();
						state = NEED_VERSION;
					} else {
						Logging::Error("HttpProtocol::ReadData(): Incorrect protocol");
						state = HTTP_DONE;
						this->error = "Protocol is not HTTP";
						ret.success = false;
						return ret;
					}
				} else {
					temp += buffer[x];
				}
				break;

			case NEED_VERSION:
				if (buffer[x] == ' ') {
					// TODO: convert this to fast_strcmp
					if (version.Set(temp)) {
						temp.Erase();
						state = NEED_STATUS_CODE;
					} else {
						Logging::Error("HttpProtocol::ReadData(): Invalid version");
						state = HTTP_DONE;
						this->error = "Unknown HTTP version";
						ret.success = false;
						return ret;
					}
				} else {
					temp += buffer[x];
				}
				break;

			case NEED_STATUS_CODE:
				if (buffer[x] == ' ') {
					if (status.Set(temp)) {
						temp.Erase();
						state = NEED_STATUS_TXT;
					} else {
						Logging::Error("HttpProtocol::ReadData(): Invalid status code");
						state = HTTP_DONE;
						this->error = "Unknown HTTP status code";
						ret.success = false;
						return ret;
					}
				} else {
					temp += buffer[x];
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
					header_key = temp;
					state = NEED_HVAL;
					temp.Erase();
				} else {
					temp += buffer[x];
				}
				break;

			case NEED_HVAL:
				if (buffer[x] == '\n') {
					//all we really care about is the content-length for now...
					if (0 == strncmp("Content-Length", header_key.ToCString(), 15)) {
						proto_content_length = strtoll(temp.ToCString(), NULL, 10);
					}
					if (0 == strncmp("Connection", header_key.ToCString(), 10)) {
						if (0 == strncmp("close", temp.ToCString(), 5)) {
							LOGGING_DEBUG("HttpProtocol::ReadData(): keepalive disabled");
							keepalive = false;
						}
					}
					state = NEED_HFIN;
					temp.Erase();
				} else {
				// we want to skip the first character IF it's a space
					if (temp.Length() == 0 && buffer[x] == ' ') {
						continue;
					}
					temp += buffer[x];
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
				if ((proto_content_length < 1) && (version.Get() != HTTP_10)) {
					LOGGING_DEBUG("HttpProtocol::ReadData(): Missing Content-Length");
					LOGGING_DEBUG("HttpProtocol::ReadData(): Response was HTTP/[%s]",
					              version.ToString().ToCString());
					state = HTTP_DONE;
					this->error = "Need Content-Length for Non-HTTP/1.0 Responses";
					ret.success = false;
					return ret;
				}
				state = NEED_BODY;
				--x;
				break;

			case NEED_BODY:
				// TODO: break this case out into a separate function
				//catch our local buffer before it overflows
				if (temp.Length() >= (sizeof(buffer) - 1)) {
					//call out for validation and reset buffer
					if (NULL != validator) {
						validator->Update(temp.ToCString(), temp.Length());
					}
					temp.Erase();
				}

				//update buffer and keep track of how many bytes we've read on the body
				temp += buffer[x];
				++parsed_content_length;

				// if a content-length response header was sent, we'll know if
				// we're supposed to be done or not
				if ((proto_content_length > 0) 
				    && (parsed_content_length >= proto_content_length)) {
					finished = true;
				}

				if (finished == true) {
					//we're done? make final validation call and return
					if (NULL != validator) {
						validator->Update(temp.ToCString(), temp.Length());
						validator->Final();
						if (true != validator->Compare(request->GetValidator())) {
							state = HTTP_DONE;
							this->error = "Validation failure: URL[";
							this->error += ((HttpRequest*)request)->GetPath();
							this->error += "] WANT[";
							this->error += request->GetValidator()->ToString();
							this->error += "] GOT[";
							this->error += validator->ToString();
							this->error += "]";
							ret.success = false;
							return ret;
						}
					}
					state = HTTP_DONE;
					ret.success = true;
					ret.state = STATE_DONE;
					ret.keepalive = keepalive;

					// end per-request time
					struct timeval time_now;
					int error;
					if (-1 == gettimeofday(&time_now, NULL)) {
						error = errno;
						char message[512];
						memset(message, 0, sizeof(message));
						strerror_r(error, message, sizeof(message));
						Logging::Error("HttpProtocol::Connect(): gettimeofday(): %s",
						               message);
					} else {
						this->end_time = (time_now.tv_sec * 1000000) + time_now.tv_usec;
						LOGGING_DEBUG("End Timer! [%" PRIu64 "]", this->end_time);
					}

					return ret;
				}
				break;

			case HTTP_DONE:
				Logging::Error("How did we end up here?");
				ret.success = true;
				ret.state = STATE_DONE;
				ret.keepalive = keepalive;

				// handle per-request time
				//gettimeofday(&end_time, NULL);

				return ret;
				break;

			default:
				Logging::Error("HttpProtocol::ReadData(): Invalid http state");
				state = HTTP_DONE;
				this->error = "Unknown error";
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
	this->error = "Error sending request";
	ret.success = false;
	return ret;
}

Network::network_error_t
HttpProtocol::Connect(Network::Socket* socket) {
	// start our per-request timer only once
	if (start_time == 0) {
		struct timeval time_now;
		int error;
		if (-1 == gettimeofday(&time_now, NULL)) {
			error = errno;
			char message[512];
			memset(message, 0, sizeof(message));
			strerror_r(error, message, sizeof(message));
			Logging::Error("HttpProtocol::Connect(): gettimeofday(): %s", message);
		} else {
			this->start_time = (time_now.tv_sec * 1000000) + time_now.tv_usec;
			LOGGING_DEBUG("Start Timer! [%" PRIu64 "]", this->start_time);
		}
	}

	// perform a protocol-specific Connect()
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
