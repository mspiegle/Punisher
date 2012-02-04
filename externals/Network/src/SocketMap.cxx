/*
 * SocketMap
 * by Michael Spiegle
 * 02.03.12
 *
 * SocketMap.cxx
 */

#include "SocketMap.hxx"
#include "Socket.hxx"
#include "Logging.hxx"

namespace Network {

SocketMap::SocketMap() {
	LOGGING_DEBUG("SocketMap::SocketMap()");
	Init();
}

SocketMap::~SocketMap() {
	LOGGING_DEBUG("SocketMap::~SocketMap()");
}

void
SocketMap::Init() {
	LOGGING_DEBUG("SocketMap::Init()");
}

void
SocketMap::Push(Socket* socket) {
	int ret = -1;
	if (-1 == (ret = pthread_mutex_lock(&lock))) {
		Logging::Error("SocketMap::Insert(): Couldn't acquire mutex");
		return;
	}
	container.push_back(socket);
	if (-1 == (ret = pthread_mutex_unlock(&lock))) {
		Logging::Error("SocketMap::Insert(): Couldn't release mutex");
	}
}

Socket*
SocketMap::Pop() {
	int ret = -1;
	Socket* socket = NULL;

	//grab lock
	if (-1 == (ret = pthread_mutex_lock(&lock))) {
		Logging::Error("SocketMap::Insert(): Couldn't acquire mutex");
		return NULL;
	}

	//if there's nothing in the container, unlock and return NULL
	if (container.size() < 1) {
		pthread_mutex_unlock(&lock);
		return NULL;
	}

	//this assumes there is something in the container, go ahead and get it
	if (NULL == (socket = container.front())) {
		//an error of some sort? unlock and return
		pthread_mutex_unlock(&lock);
		return NULL;
	}

	//if we got this far, we should be able to pop the element we just got
	//and return to our caller
	container.pop_front();

	if (-1 == (ret = pthread_mutex_unlock(&lock))) {
		Logging::Error("SocketMap::Insert(): Couldn't release mutex");
	}
	return socket;
}

}
