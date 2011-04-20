/*
 * Punisher
 * by Michael Spiegle
 * 09.03.09
 *
 * Client.cxx
 */

#include "Client.hxx"
#include "Config.hxx"
#include "Worker.hxx"
#include "Logging.hxx"
#include "String.hxx"

#include <stdio.h>
#include <vector>
#include <sys/time.h>
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
	Statistics tmp_stats;
	uint64_t last_requests = 0;
	uint64_t last_duration = 0;
	int finished_threads = 0;

	// get start time
	struct timeval start_time;
	struct timeval now_time;
	if (-1 == gettimeofday(&start_time, NULL)) {
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
			printf("\n%7s %6s %5s %5s %5s %8s %10s %6s %12s %6s\n",
			       "SECS",
			       "RPS",
			       "OSKS",
			       "CSKS",
			       "KSKS",
			       "SENT",
			       "RECV",
			       "MSEC",
			       "TRQS",
			       "FRQS");
		}

		//iterate to find terminated threads
		for (i = workers.begin(); i != workers.end(); ++i) {

			//collect (possibly) final stats for this thread
			tmp_stats += (*i)->GetStatistics();

			// are we still running?
			if (false == (*i)->GetRunning()) {
				// TODO: we might want to check the return value
				pthread_cancel((*i)->GetThread());
				++finished_threads;
			}
		}

		//get current time and display the aggregated stats
		if (-1 == gettimeofday(&now_time, NULL)) {
			Logging::Error("Client::Start(): Error getting start time");
			return;
		}
		uint64_t elapsed_requests = tmp_stats.GetTotalRequests() - last_requests;
		uint64_t elapsed_duration = tmp_stats.GetRequestDuration() - last_duration;
		if (elapsed_requests == 0) {
			//TODO: not an ideal solution, but should stop the FPE
			elapsed_requests = 1;
		}
		printf("%7ld %6" PRIu64 " %5d %5d %5d %8" PRIu64 " %10" PRIu64 \
		       " %6" PRIu64 " %12" PRIu64 " %6" PRIu64 "\n",
		       now_time.tv_sec - start_time.tv_sec,
		       elapsed_requests / config->GetDelay(),
		       tmp_stats.GetOpenSockets(),
		       tmp_stats.GetConnectedSockets(),
		       tmp_stats.GetKeepaliveSockets(),
		       tmp_stats.GetBytesSent(),
		       tmp_stats.GetBytesReceived(),
		       (elapsed_duration / elapsed_requests) / 1000,
		       tmp_stats.GetTotalRequests(),
		       tmp_stats.GetFailedRequests());
		last_requests = tmp_stats.GetTotalRequests();
		last_duration = tmp_stats.GetRequestDuration();
		tmp_stats.Reset();

		//if all the threads have been joined, we can exit
		if (finished_threads >= config->GetThreads()) {
			running = false;
		}
	}
}

}
