/*
 * Socket
 * by Michael Spiegle
 * 09.18.09
 *
 * LocalSocket.cxx
 */

#include "LocalSocket.hxx"
#include "Logging.hxx"

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

namespace Network {

LocalSocket::LocalSocket() {
	LOGGING_DEBUG("LocalSocket::LocalSocket()");
	memset(path, 0, sizeof(path));
}

LocalSocket::~LocalSocket() {
	LOGGING_DEBUG("LocalSocket::~LocalSocket()");
}

bool
LocalSocket::Close() {
	LOGGING_DEBUG("LocalSocket::Close()");
	if (strlen(path) > 0) {
		unlink(path);
	}
	return Socket::Close();
}

bool
LocalSocket::Create(int type, int protocol) {
	return Socket::Create(Network::Unix, type, protocol);
}

LocalSocket*
LocalSocket::Accept() {
	return static_cast<LocalSocket*>(Socket::Accept());
}

}
