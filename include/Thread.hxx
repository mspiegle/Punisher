/*
 * Punisher
 * by Michael Spiegle
 * 09.03.09
 *
 * Thread.hxx
 */

#ifndef _PUNISHER_THREAD_HXX_
#define _PUNISHER_THREAD_HXX_

#include <pthread.h>

namespace Punisher {

class Thread {
	private:
		pthread_t     thread;
		volatile bool running;
	
	protected:
		unsigned int  id;

	public:
		Thread();
		virtual ~Thread();

		int  GetId() const;
		void SetId(int id);

		inline bool GetRunning() const {
			return running;
		}
		inline void SetRunning(bool running) {
			this->running = running;
		}

		bool StartThread();
		static void* ThreadHelper(void* ptr);
		virtual void* ThreadMain() = 0;

		inline pthread_t GetThread() const {
			return thread;
		}
};

}

#endif
