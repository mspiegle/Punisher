/*
 * Socket
 * by Michael Spiegle
 * 09.18.09
 *
 * LocalSocket.h
 */

#ifndef _NETWORK_LOCALSOCKET_H_
#define _NETWORK_LOCALSOCKET_H_

#include "Socket.h"

#include <sys/types.h>
#include <sys/socket.h>

namespace Network {

class LocalSocket : public Socket {
	private:
		char path[1024];

	public:
		LocalSocket();
		~LocalSocket();

		bool Close();
		bool Create(int type, int protocol);
		LocalSocket* Accept();
};

}

#endif
