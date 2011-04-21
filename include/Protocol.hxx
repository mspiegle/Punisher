/*
 * Punisher
 * by Michael Spiegle
 * 10.15.09
 *
 * Protocol.hxx
 */

#ifndef _PUNISHER_PROTOCOL_HXX_
#define _PUNISHER_PROTOCOL_HXX_

#include "Validator.hxx"
#include "String.hxx"
#include "Network.hxx"
#include "Socket.hxx"

#include <stddef.h>
#include <sys/time.h>
#include <stdint.h>

namespace Punisher {

//this is bogus... C++ is making me fwd declare my class
//instead of just letting me #include it.
class Request;

typedef enum {
	STATE_NONE = 0,
	STATE_DONE,
	STATE_READ,
	STATE_WRITE
} protocol_state_t;

typedef struct {
	bool             success;
	bool             keepalive;
	size_t           bytes_transferred;
	protocol_state_t state;
} protocol_result_t;

class Protocol {
	protected:
		const Request*     request;
		mutable Validator* validator;
		M::String          error;
		uint64_t           start_time;
		uint64_t           end_time;

	public:
		Protocol();
		Protocol(const Request* request);
		virtual ~Protocol();

		uint64_t GetStartTime() const {
			return start_time;
		}

		uint64_t GetEndTime() const {
			return end_time;
		}

		uint64_t GetRequestDuration() const {
			return end_time - start_time;
		}

		Validator* GetValidator() const;
		void       SetValidator(Validator* validator);

		const Request* GetRequest() const;

		inline M::String GetError() const {
			return error;
		}

		virtual protocol_result_t ReadData(Network::Socket* socket) = 0;
		virtual protocol_result_t WriteData(Network::Socket* socket) = 0;
		virtual Network::network_error_t Connect(Network::Socket* socket) = 0;
		virtual Network::socket_type_t GetSocketType() = 0;
};

}

#endif
