/*
 * Punisher
 * by Michael Spiegle
 * 05.11.10
 *
 * Statistics.cxx
 */

#include "Statistics.hxx"

namespace Punisher {

Statistics::Statistics() {
	open_sockets = 0;
	connected_sockets = 0;
	total_requests = 0;
	failed_requests = 0;
	concurrent_requests = 0;
	bytes_sent = 0;
	bytes_received = 0;
}

Statistics::~Statistics() {
}

void
Statistics::Print() {
}

}
