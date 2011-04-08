/*
 * HostPort 
 * by Michael Spiegle
 * 10.28.09
 *
 * HostPort.hxx
 */

#ifndef _NETWORK_HOSTPORT_HXX_
#define _NETWORK_HOSTPORT_HXX_

#include "String.hxx"

#include <stdint.h>

namespace Network {

class HostPort {
	private:
		uint16_t port;
	public:
		HostPort();
		~HostPort();

		bool Set(uint16_t port);
		uint16_t Get() const;

		M::String ToString() const;
		uint16_t ToNetwork() const;
};

}

#endif
