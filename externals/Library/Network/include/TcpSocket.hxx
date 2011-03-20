/*
 * Socket
 * by Michael Spiegle
 * 09.04.09
 *
 * TcpSocket.hxx
 */

#ifndef _NETWORK_TCPSOCKET_HXX_
#define _NETWORK_TCPSOCKET_HXX_

#include "InetSocket.hxx"

namespace Network {

class TcpSocket : public InetSocket {
  private:
  public:
    TcpSocket();
		TcpSocket(int protocol);
    ~TcpSocket();

		bool Create(int protocol);
		TcpSocket* Accept();
};

}

#endif
