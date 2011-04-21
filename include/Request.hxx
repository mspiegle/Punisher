/*
 * Punisher
 * by Michael Spiegle
 * 09.03.09
 *
 * Request.hxx
 */

#ifndef _PUNISHER_REQUEST_HXX_
#define _PUNISHER_REQUEST_HXX_

#include "Validator.hxx"
#include "Host.hxx"
#include "String.hxx"
#include "Socket.hxx"

namespace Punisher {

//this is bogus... C++ is making me fwd declare my class
//instead of just letting me #include it.
class Protocol;

class Request {
	protected:
		bool               processed;
		M::String          request;
		Network::Host      host;
		mutable Validator* validator;

	public:
		Request();
		virtual ~Request();

		Validator*& GetValidator() const;

		const Network::Host& GetHost() const;
		Network::Host& GetHost();

		//virtual const M::String& GetRequest() const = 0;
		//virtual bool Process() = 0;
		virtual Protocol* CreateProtocol() const = 0;

		virtual bool SetField(int field, const char* value) = 0;
};

}

#endif
