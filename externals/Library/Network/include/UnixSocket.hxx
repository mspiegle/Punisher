/*
 * Socket
 * by Michael Spiegle
 * 09.04.09
 *
 * UnixSocket.hxx
 */

#ifndef _NETWORK_UNIXSOCKET_HXX_
#define _NETWORK_UNIXSOCKET_HXX_

#include "LocalSocket.hxx"

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
