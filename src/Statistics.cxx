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

Statistics&
Statistics::operator+=(const Statistics& arg) {
	this->open_sockets += arg.GetOpenSockets();
	this->connected_sockets += arg.GetConnectedSockets();
	this->total_requests += arg.GetTotalRequests();
	this->failed_requests += arg.GetFailedRequests();
	this->concurrent_requests += arg.GetConcurrentRequests();
	this->bytes_sent += arg.GetBytesSent();
	this->bytes_received += arg.GetBytesReceived();
	return *this;
}

}
