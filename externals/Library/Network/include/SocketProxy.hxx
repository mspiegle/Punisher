/*
 * SocketProxy
 * by Michael Spiegle
 * 01.21.10
 *
 * SocketProxy.h
 */

#ifndef _NETWORK_SOCKETPROXY_H_
#define _NETWORK_SOCKETPROXY_H_

#include "Network.hxx"
#include "Socket.hxx"

#include <sys/types.h>
#include <sys/socket.h>

namespace Network {

class SocketProxy : private Socket {
	private:
		Socket* socket;

	public:
		SocketProxy();
		SocketProxy(Socket* socket);
		virtual ~SocketProxy();

		void* GetNotes() const;
		void  SetNotes(void* notes);

		network_error_t Read(void* buffer, size_t* size);
		network_error_t Write(const void* buffer, size_t* size);
	};

}

#endif
