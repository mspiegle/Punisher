/*
 * Resolve
 * by Michael Spiegle
 * 10.30.09
 *
 * Resolve.h
 */

#ifndef _NETWORK_RESOLVE_H_
#define _NETWORK_RESOLVE_H_

#include "String.h"

#include <netinet/in.h>

namespace Network {

	namespace Resolve {
		bool NameToAddress(const M::String& name, struct in_addr* address);
		bool AddressToName(const struct in_addr* address, M::String& name);
	}

}

#endif
