/*
 * Socket
 * by Michael Spiegle
 * 09.07.09
 *
 * UnixSocket.cxx
 */

#include "LocalSocket.hxx"
#include "UnixSocket.hxx"

namespace Network {

UnixSocket::UnixSocket() { }
UnixSocket::~UnixSocket() { }

bool
UnixSocket::Create(int protocol) {
	return LocalSocket::Create(Network::Stream, protocol);
}

UnixSocket*
UnixSocket::Accept() {
	return static_cast<UnixSocket*>(LocalSocket::Accept());
}

}
