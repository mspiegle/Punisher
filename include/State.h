/*
 * Punisher
 * by Michael Spiegle
 * 10.15.09
 *
 * State.h
 */

#ifndef _PUNISHER_STATE_H_
#define _PUNISHER_STATE_H_

#include "Validator.h"
#include "String.h"
#include "Network.h"
#include "Socket.h"

#include <stddef.h>

namespace Punisher {

//this is bogus... C++ is making me fwd declare my class
//instead of just letting me #include it.
class Request;

//these states help the caller figure out the following:
//STATE_FAILURE - there was a protocol error of some sort, perform cleanup
//STATE_DONE - the protocol is done successfully, perform cleanup
//STATE_READ - the protocol parser wants to receive data
//STATE_WRITE - the protocol parser wants to write data
typedef enum {
	MODE_NONE = 0,
	MODE_DONE,
	MODE_READ,
	MODE_WRITE
} state_mode_t;

typedef struct {
	bool         success;
	bool         keepalive;
	state_mode_t mode;
	size_t       bytes_transferred;
} state_result_t;

class State {
	protected:
		const Request*     request;
		mutable Validator* validator;
		M::String          error;

	public:
		State();
		State(const Request* request);
		virtual ~State();


		const Request* GetRequest() const;
		Validator*& GetValidator() const;
		inline M::String GetError() const {
			return error;
		}

		virtual state_result_t ReadData(Network::Socket* socket) = 0;
		virtual state_result_t WriteData(Network::Socket* socket) = 0;
		virtual Network::network_error_t Connect(Network::Socket* socket) = 0;
};

}

#endif
