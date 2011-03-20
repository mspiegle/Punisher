/*
 * SocketProxy
 * by Michael Spiegle
 * 01.21.10
 *
 * SocketProxy.cxx
 */

#include "Network.hxx"
#include "SocketProxy.hxx"
#include "Logging.hxx"

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>

namespace Network {

SocketProxy::SocketProxy() {
	LOGGING_DEBUG("SocketProxy::SocketProxy()");
	socket = NULL;
}

SocketProxy::SocketProxy(Socket* socket) {
	this->socket = socket;
}

SocketProxy::~SocketProxy() {
	LOGGING_DEBUG("SocketProxy::~SocketProxy()");
}

void*
SocketProxy::GetNotes() const {
	LOGGING_DEBUG("SocketProxy::GetNotes()");
	if (NULL != socket) {
		return socket->GetNotes();
	}
	Logging::Error("SocketProxy::GetNotes(): socket not defined");
	return NULL;
}

void
SocketProxy::SetNotes(void* notes) {
	LOGGING_DEBUG("SocketProxy::SetNotes(void* notes)");
	if (NULL != socket) {
		return socket->SetNotes(notes);
	}
	Logging::Error("SocketProxy::SetNotes(): socket not defined");
}

network_error_t
SocketProxy::Read(void* buffer, size_t* size) {
	LOGGING_DEBUG("SocketProxy::Read(%p, %lu)", buffer, *size);
	if (NULL != socket) {
		return socket->Read(buffer, size);
	}
	Logging::Error("SocketProxy::Read(): socket not defined");
	return NETWORK_ERROR;
}

network_error_t
SocketProxy::Write(const void* buffer, size_t* size) {
	LOGGING_DEBUG("SocketProxy::Write(%p, %lu)", buffer, *size);
	if (NULL != socket) {
		return socket->Write(buffer, size);
	}
	Logging::Error("SocketProxy::Write(): socket not defined");
	return NETWORK_ERROR;
}

}
