/*
 * Socket
 * by Michael Spiegle
 * 09.04.09
 *
 * UdpSocket.hxx
 */

#ifndef _NETWORK_UDPSOCKET_HXX_
#define _NETWORK_UDPSOCKET_HXX_

#include "InetSocket.hxx"

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
