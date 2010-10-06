/*
 * Punisher
 * by Michael Spiegle
 * 09.03.09
 *
 * Client.h
 */

#ifndef _PUNISHER_CLIENT_H_
#define _PUNISHER_CLIENT_H_

#include "Worker.h"
#include "Config.h"
#include "String.h"

#include <vector>
#include <signal.h>

namespace Punisher {

class Client {
	private:
		Config*              config;
		M::String            error;
		std::vector<Worker*> workers;
		volatile bool        running;

	public:
		Client();
		~Client();

		bool SetConfig(Config* config);
		void Start();
		void Stop();
		static void ExitHandler(int signal);

		M::String GetError();
};

}

#endif
