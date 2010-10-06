/*
 * Punisher
 * by Michael Spiegle
 * 09.03.09
 *
 * Client.cpp
 */

#include "Client.h"
#include "Config.h"
#include "Worker.h"
#include "Logging.h"
#include "String.h"

#include <stdio.h>
#include <vector>
#include <time.h>
#include <signal.h>
#include <unistd.h>

#define __STDC_FORMAT_MACROS
#include <inttypes.h>

namespace Punisher {

static const void* client_address;
static bool stopping;

Client::Client() {
	client_address = this;
	running = true;
	stopping = false;
}

Client::~Client() {
	std::vector<Worker*>::const_iterator i;
	for (i = workers.begin(); i != workers.end(); ++i) {
		delete(*i);
	}
}

M::String
Client::GetError() {
	return error;
}

bool
Client::SetConfig(Config* config) {
	this->config = config;
	if (config->GetRequests().size() < 1) {
		error = "No requests defined";
		return false;
	}
	return true;
}

void
Client::ExitHandler(int signal) {
	//TODO: I should probably redo this because if the user
	//hits ctrl-c twice fast, then we have issues.
	if (SIGINT == signal && stopping == false) {
		stopping = true;
		((Punisher::Client*)client_address)->Stop();
	}
}

void
Client::Stop() {
	Logging::Info("Stopping application");

	//stop all threads
	std::vector<Worker*>::const_iterator i;
	for (i = workers.begin(); i != workers.end(); ++i) {
		(*i)->SetRunning(false);
	}

	//wait for an interval
	//sleep(config->GetDelay());
	//wait for a second
	sleep(1);

	//display all errors
	if (config->GetErrors()) {
		printf("Errors:\n");
		for (i = workers.begin(); i != workers.end(); ++i) {
			//then we display errors logged
			std::deque<M::String> worker_errors = (*i)->GetErrors();
			std::deque<M::String>::iterator i;
			for (i = worker_errors.begin(); i != worker_errors.end(); ++i) {
				printf("%s\n", (*i).ToCString());
			}
		}
	}
}

void
Client::Start() {
	Logging::Info("Starting application with: %d threads", config->GetThreads());

	//capture ctrl-c
	struct sigaction exit_handler;
	exit_handler.sa_handler = this->ExitHandler;
	exit_handler.sa_flags = 0;
	sigemptyset(&(exit_handler.sa_mask));
	sigaction(SIGINT, &exit_handler, NULL);

	//kick off threads.  default to the number of cores detected
	int x;
	for (x = 0; x < config->GetThreads(); ++x) {
		Worker* worker = new Worker();
		worker->SetConfig(config);
		worker->SetId(x);
		worker->SetRunning(true);
		worker->SetIterations(config->GetIterations() / config->GetThreads());
		worker->StartThread();
		workers.push_back(worker);
	}

	//start a forever loop with counter for header display
	int count;
	std::vector<Worker*>::const_iterator i;
	uint64_t tmp_requests = 0;
	uint64_t tmp_failed_requests = 0;
	uint64_t tmp_bytes_sent = 0;
	uint64_t tmp_bytes_received = 0;
	uint64_t last_requests = 0;
	int tmp_open_sockets = 0;
	int tmp_connected_sockets = 0;
	int tmp_keepalive_sockets = 0;
	int joined_threads = 0;
	
	//get the start time
	struct timespec start_time;
	struct timespec now_time;
	if (-1 == clock_gettime(CLOCK_REALTIME, &start_time)) {
		Logging::Error("Client::Start(): Error getting start time");
		return;
	}

	//start the loop
	for(count = 0; running == true; ++count) {
		//do our sleep, and sometimes print column headers
		sleep(config->GetDelay());

		//silence output
		if (config->GetQuiet()) {
			continue;
		}

		//every 10 iterations, print the headers again
		if (0 == (count % 10)) {
			printf("\n%7s %7s %6s %6s %6s %8s %8s %12s %8s\n",
			       "SECS",
			       "RPS",
			       "OSKS",
			       "CSKS",
			       "KSKS",
			       "SENT",
			       "RECV",
			       "TRQS",
			       "FRQS");
		}

		//iterate to find terminated threads
		for (i = workers.begin(); i != workers.end(); ++i) {

			//collect (possibly) final stats for this thread
			tmp_requests += (*i)->GetRequests();
			tmp_failed_requests += (*i)->GetFailedRequests();
			tmp_open_sockets += (*i)->GetOpenSockets();
			tmp_connected_sockets += (*i)->GetConnectedSockets();
			tmp_keepalive_sockets += (*i)->GetKeepaliveSockets();
			tmp_bytes_sent += (*i)->GetStatistics().GetBytesSent();
			tmp_bytes_received += (*i)->GetStatistics().GetBytesReceived();

			//check for termination
			if (0 == pthread_tryjoin_np((*i)->GetThread(), NULL)) {
				++joined_threads;
			}
		}

		//get current time and display the aggregated stats
		if (-1 == clock_gettime(CLOCK_REALTIME, &now_time)) {
			Logging::Error("Client::Start(): Error getting current time");
			return;
		}
		printf("%7ld %7" PRIu64 " %6d %6d %6d %8" PRIu64 " %8" PRIu64 \
		       " %12" PRIu64 " %8" PRIu64 "\n",
		       now_time.tv_sec - start_time.tv_sec,
		       (tmp_requests - last_requests) / config->GetDelay(),
		       tmp_open_sockets,
		       tmp_connected_sockets,
		       tmp_keepalive_sockets,
		       tmp_bytes_sent,
					 tmp_bytes_received,
		       tmp_requests,
		       tmp_failed_requests);
		last_requests = tmp_requests;
		tmp_requests = 0;
		tmp_failed_requests = 0;
		tmp_open_sockets = 0;
		tmp_connected_sockets = 0;
		tmp_keepalive_sockets = 0;
		tmp_bytes_sent = 0;
		tmp_bytes_received = 0;
	
		//if all the threads have been joined, we can exit
		if (joined_threads >= config->GetThreads()) {
			running = false;
		}
	}
}

}
