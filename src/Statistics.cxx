/*
 * Punisher
 * by Michael Spiegle
 * 05.11.10
 *
 * Statistics.cxx
 */

#include "Statistics.hxx"
#include "Logging.hxx"

namespace Punisher {

Statistics::Statistics() {
	Reset();
}

Statistics::~Statistics() {
}

void Statistics::Reset() {
	open_sockets = 0;
	connected_sockets = 0;
	keepalive_sockets = 0;
	total_requests = 0;
	failed_requests = 0;
	concurrent_requests = 0;
	bytes_sent = 0;
	bytes_received = 0;
}

void
Statistics::Print() {
	Logging::Info("open_sockets: %d", this->GetOpenSockets());
	Logging::Info("connected_sockets: %d", this->GetConnectedSockets());
	Logging::Info("total_requests: %d", this->GetTotalRequests());
	Logging::Info("failed_requests: %d", this->GetFailedRequests());
	Logging::Info("concurrent_requests: %d", this->GetConcurrentRequests());
	Logging::Info("bytes_sent: %d", this->GetBytesSent());
	Logging::Info("bytes_received: %d", this->GetBytesReceived());
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

Statistics&
Statistics::operator=(const Statistics& arg) {
	this->open_sockets = arg.GetOpenSockets();
	this->connected_sockets = arg.GetConnectedSockets();
	this->total_requests = arg.GetTotalRequests();
	this->failed_requests = arg.GetFailedRequests();
	this->concurrent_requests = arg.GetConcurrentRequests();
	this->bytes_sent = arg.GetBytesSent();
	this->bytes_received = arg.GetBytesReceived();
	return *this;
}

}
