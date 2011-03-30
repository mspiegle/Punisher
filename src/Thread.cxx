/*
 * Punisher
 * by Michael Spiegle
 * 09.03.09
 *
 * Thread.cxx
 */

#include "Thread.hxx"
#include "Logging.hxx"

namespace Punisher {

Thread::Thread() {
	running = false;
}

Thread::~Thread() {
}

int
Thread::GetId() const {
	return id;
}
void
Thread::SetId(int id) {
	this->id = id;
}

void*
Thread::ThreadHelper(void* ptr) {
	LOGGING_DEBUG("ThreadHelper()");
	return ((Thread*)ptr)->ThreadMain();
}

bool
Thread::StartThread() {
	if (0 == pthread_create(&thread, NULL, ThreadHelper, this)) {
		LOGGING_DEBUG("Spawned thread");
		return true;
	}
	LOGGING_DEBUG("Error spawning thread");
	return false;
}

}
