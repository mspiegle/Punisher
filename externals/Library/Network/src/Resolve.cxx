/*
 * Resolve
 * by Michael Spiegle
 * 10.30.09
 *
 * Resolve.cpp
 */

#include "Resolve.h"
#include "Logging.h"
#include "String.h"

#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

namespace Network {

namespace Resolve {

bool
NameToAddress(const M::String& name, struct in_addr* address) {
	LOGGING_DEBUG("Resolve::NameToAddress()");
	struct addrinfo* result = NULL;
	if (0 != getaddrinfo(name.ToCString(), NULL, NULL, &result)) {
		Logging::Error("Resolve::NameToAddress(): Can't resolve [%s]",
									 name.ToCString());
		return false;
	}
	
	struct sockaddr_in* addr = (struct sockaddr_in*)result->ai_addr;
	memcpy(address, &(addr->sin_addr), sizeof(struct in_addr));
	freeaddrinfo(result);
	return true;
}

bool
AddressToName(const struct in_addr* address, M::String& name) {
	//TODO
	(void)address;
	(void)name;
	return true;
}

} //Resolve::
} //Network::
