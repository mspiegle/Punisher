/*
 * Socket
 * by Michael Spiegle
 * 09.04.09
 *
 * UdpSocket.h
 */

#ifndef _NETWORK_UDPSOCKET_H_
#define _NETWORK_UDPSOCKET_H_

#include "InetSocket.h"

namespace Network {

class UdpSocket : public InetSocket {
  private:
  public:
    UdpSocket();
    ~UdpSocket();

		bool Create(int protocol);
		UdpSocket* Accept();
};

}

#endif
