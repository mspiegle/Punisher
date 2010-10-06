/*
 * HostPort 
 * by Michael Spiegle
 * 10.28.09
 *
 * HostPort.h
 */

#ifndef _NETWORK_HOSTPORT_H_
#define _NETWORK_HOSTPORT_H_

#include "String.h"

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
