/*
 * Socket
 * by Michael Spiegle
 * 09.06.09
 *
 * Socket.cxx
 */

#include "Socket.hxx"
#include "Logging.hxx"
#include "Network.hxx"

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>

namespace Network {

Socket::Socket() {
	LOGGING_DEBUG("Socket::Socket()");
	Init();
}

Socket::Socket(int fd) {
	LOGGING_DEBUG("Socket::Socket(int fd)");
	Init();
	if (fd > 0) {
		this->fd = fd;
	}
}

Socket::~Socket() {
	LOGGING_DEBUG("Socket::~Socket()");
	Close();
}

void
Socket::Init() {
	LOGGING_DEBUG("Socket::Init()");
	fd = -1;
	notes = NULL;
	listening = false;
	connected = false;
}

int
Socket::GetFd() const {
	LOGGING_DEBUG("Socket::GetFd()");
	return fd;
}

void
Socket::SetFd(int fd) {
	LOGGING_DEBUG("Socket::SetFd(int fd)");
	this->fd = fd;
}

void*
Socket::GetNotes() const {
	LOGGING_DEBUG("Socket::GetNotes()");
	return notes;
}

void
Socket::SetNotes(void* notes) {
	LOGGING_DEBUG("Socket::SetNotes(void* notes)");
	this->notes = notes;
}

network_error_t
Socket::Read(void* buffer, size_t* size) {
	LOGGING_DEBUG("Socket::Read(%p, %lu)", buffer, *size);
	int br = 0;
	if (-1 == (br = read(fd, buffer, *size))) {
		if (errno == EWOULDBLOCK) {
			return NETWORK_WOULDBLOCK;
		}
		Logging::Error("Socket::Read(): %s", strerror(errno));
		return NETWORK_ERROR;
	}
	*size = br;
	return NETWORK_SUCCESS;
}

network_error_t
Socket::Write(const void* buffer, size_t* size) {
	LOGGING_DEBUG("Socket::Write(%p, %lu)", buffer, *size);
	int bw = 0;
	if (-1 == (bw = write(fd, buffer, *size))) {
		if (errno == EWOULDBLOCK) {
			return NETWORK_WOULDBLOCK;
		}
		Logging::Error("Socket::Write(): %s", strerror(errno));
		return NETWORK_ERROR;
	}
	*size = bw;
	return NETWORK_SUCCESS;
}

int
Socket::GetStatusFlags() const {
	LOGGING_DEBUG("Socket::GetStatusFlags()");
	if (fd > 0) {
		return fcntl(fd, F_GETFL, 0);
	}
	LOGGING_DEBUG("Socket::GetStatusFlags(): Error");
	return -1;
}

bool
Socket::GetBlocking() const {
	LOGGING_DEBUG("Socket::GetBlocking()");
	int flags;
	if (-1 == (flags = GetStatusFlags())) {
		Logging::Error("Socket::GetBlocking(): GetStatusFlags() error");
		return false;
	}
	LOGGING_DEBUG("Socket::GetBlocking(): Got flags [%d]", flags);
	return (flags & O_NONBLOCK) ? false : true;
}

bool
Socket::SetBlocking(bool blocking) {
	LOGGING_DEBUG("Socket::SetBlocking(%s)", blocking ? "true" : "false");
	int flags;
	if (-1 == (flags = GetStatusFlags())) {
		Logging::Error("Socket::SetBlocking(): GetStatusFlags() error");
		return false;
	}
	LOGGING_DEBUG("Socket::SetBlocking(): Got flags [%d]", flags);
	if (blocking) {
		flags &= ~O_NONBLOCK;
	} else {
		flags |= O_NONBLOCK;
	}
	return ( 0 == fcntl(fd, F_SETFL, flags)) ? true : false;
}

bool
Socket::SetReuseAddr(bool reuse) {
	int ra;
	if (reuse) {
		ra = 1;
	} else {
		ra = 0;
	}
	if (-1 == setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &ra, sizeof(&ra))) {
		return false;
	}
	return true;
}

bool
Socket::Create(int domain, int type, int protocol) {
	LOGGING_DEBUG("Socket::Create()");
	if (-1 == (fd = socket(domain, type, protocol))) {
		return false;
	}
	return true;
}

bool
Socket::Listen(int backlog) {
	LOGGING_DEBUG("Socket::Listen(int backlog)");
	//TODO:
	//Why do we need to set, then unset listening? clean it up
	if (fd > 0) {
		listening = true;
		return (0 == listen(fd, backlog)) ? true : false;
	}
	listening = false;
	return false;
}

bool
Socket::Listening() const {
	return listening;
}

Socket*
Socket::Accept() {
	LOGGING_DEBUG("Socket::Accept()");
	int fd = -1;
	if (-1 == (fd = accept(this->fd, NULL, NULL))) {
		//TODO:
		//Throw exception
		return NULL;
	}
	Socket* socket = new Socket(fd);
	return socket;
}

bool
Socket::Close() {
	LOGGING_DEBUG("Socket::Close()");
  if (fd > 0) {
    if (0 == close(fd)) {
      fd = -1;
			connected = false;
			listening = false;
      return true;
    }
  }
  return false;
}

network_error_t
Socket::Bind(const struct sockaddr* addr, socklen_t size) {
	LOGGING_DEBUG("Socket::Bind()");
	if (-1 == bind(fd, addr, size)) {
		Logging::Error("Socket::Bind(): %s", strerror(errno));
		return NETWORK_ERROR;
	}
	return NETWORK_SUCCESS;
}

network_error_t
Socket::Connect(const struct sockaddr* addr, socklen_t size) {
	LOGGING_DEBUG("Socket::Connect()");
	if (-1 == connect(fd, addr, size)) {
		if (errno == EINPROGRESS) {
			LOGGING_DEBUG("Socket::Connect(): Connect would block");
			return NETWORK_WOULDBLOCK;
		}
		Logging::Error("Socket::Connect(): %s", strerror(errno));
		return NETWORK_ERROR;
	}
	connected = true;
	return NETWORK_SUCCESS;
}

bool
Socket::GetConnected() const {
	return connected;
}

}
