/*
 * HostName 
 * by Michael Spiegle
 * 10.28.09
 *
 * HostName.hxx
 */

#ifndef _NETWORK_HOSTNAME_HXX_
#define _NETWORK_HOSTNAME_HXX_

#include "String.hxx"

namespace Network {

class HostName {
	private:
		M::String name;

	public:
		HostName();
		~HostName();

		bool Set(const M::String& name);
		bool Set(const char* name);

		const M::String& ToString() const;
};

}

#endif
