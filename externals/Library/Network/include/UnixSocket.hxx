/*
 * Socket
 * by Michael Spiegle
 * 09.04.09
 *
 * UnixSocket.h
 */

#ifndef _NETWORK_UNIXSOCKET_H_
#define _NETWORK_UNIXSOCKET_H_

#include "LocalSocket.h"

namespace Network {

class UnixSocket : public LocalSocket {
  private:
  public:
    UnixSocket();
    ~UnixSocket();

		bool Create(int domain);
		UnixSocket* Accept();
};

}

#endif
