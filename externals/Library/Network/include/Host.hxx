/*
 * Host
 * by Michael Spiegle
 * 10.28.09
 *
 * Host.h
 */

#ifndef _NETWORK_HOST_H_
#define _NETWORK_HOST_H_

#include "HostAddress.hxx"
#include "HostPort.hxx"
#include "HostName.hxx"

#include <string>

namespace Network {

class Host {
	private:
		HostAddress address;
		HostPort    port;
		HostName    name;
	public:
		Host();
		~Host();

		bool SetName(const char* name);
		bool SetName(const M::String& name);

		bool SetAddress(const char* address);
		bool SetAddress(const M::String& address);
		bool SetAddress(const in_addr& address);

		bool SetPort(uint16_t port);

		const HostAddress& Address() const;
		const HostPort& Port() const;
		const HostName& Name() const;

		M::String ToString() const;
};

}

#endif
