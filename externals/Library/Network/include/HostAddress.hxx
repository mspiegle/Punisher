/*
 * HostAddress 
 * by Michael Spiegle
 * 10.28.09
 *
 * HostAddress.hxx
 */

#ifndef _NETWORK_HOSTADDRESS_HXX_
#define _NETWORK_HOSTADDRESS_HXX_

#include "String.hxx"

#include <arpa/inet.h>

namespace Network {

class HostAddress {
	private:
		struct in_addr address;

	public:
		HostAddress();
		~HostAddress();

		bool Set(const struct in_addr& address);
		bool Set(const char* address);
		const struct in_addr& Get() const;

		M::String ToString() const;
};

}

#endif
