/*
 * Socket
 * by Michael Spiegle
 * 09.04.09
 *
 * TcpSocket.h
 */

#ifndef _NETWORK_TCPSOCKET_H_
#define _NETWORK_TCPSOCKET_H_

#include "InetSocket.h"

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
