/*
 * Punisher
 * by Michael Spiegle
 * 10.12.09
 *
 * Worker.hxx
 */

#ifndef _PUNISHER_WORKER_HXX_
#define _PUNISHER_WORKER_HXX_

#include "Config.hxx"
#include "IHandler.hxx"
#include "PollManager.hxx"
#include "Thread.hxx"
#include "Item.hxx"
#include "String.hxx"
#include "Statistics.hxx"
#include "SocketMap.hxx"

#include <stdint.h>
#include <deque>

namespace Punisher {

class Worker : public Thread, public Event::IHandler {
	private:
		size_t                iterations;
		uint64_t              next_request;
		const Config*         config;
		Statistics            stats;
		Event::PollManager    manager;
		std::deque<M::String> errors;
		Network::SocketMap    keepalives;

	public:
		Worker();
		~Worker();

		const Config* GetConfig() const;
		void          SetConfig(const Config* config);

		const Statistics& GetStatistics() const;

		void SetIterations(size_t iterations);

		//thread stuff
		void* ThreadMain();

		//event handler stuff
		void HandleReadable(const Event::Item& item);
		void HandleWriteable(const Event::Item& item);
		void HandleHangup(const Event::Item& item);
		void HandleError(const Event::Item& item);
		void HandleAcceptable(const Event::Item& item);

		int GetKeepaliveSockets();

		//error handling
		inline const std::deque<M::String>& GetErrors() const {
			return errors;
		}
};

}

#endif
