/*
 * Punisher
 * by Michael Spiegle
 * 05.10.10
 *
 * Statistics.h
 */

#ifndef _PUNISHER_STATISTICS_H_
#define _PUNISHER_STATISTICS_H_

#include <stdint.h>

namespace Punisher {

class Statistics {
	private:
		//sockets
		int open_sockets;
		int connected_sockets;

		//requests
		uint64_t total_requests;
		uint64_t failed_requests;
		int      concurrent_requests;

		//data
		uint64_t bytes_sent;
		uint64_t bytes_received;

	public:
		Statistics();
		virtual ~Statistics();

		//sockets
		inline int GetOpenSockets() const {
			return open_sockets;
		}

		inline void SetOpenSockets(int open_sockets) {
			this->open_sockets = open_sockets;
		}

		inline int AddOpenSockets(int open_sockets) {
			return this->open_sockets += open_sockets;
		}

		inline int GetConnectedSockets() const {
			return connected_sockets;
		}

		inline void SetConnectedSockets(int connected_sockets) {
			this->connected_sockets = connected_sockets;
		}

		inline int AddConnectedSockets(int connected_sockets) {
			return this->connected_sockets += connected_sockets;
		}

		//requests
		inline uint64_t GetTotalRequests() const {
			return total_requests;
		}

		inline void SetTotalRequests(uint64_t total_requests) {
			this->total_requests = total_requests;
		}

		inline uint64_t AddTotalRequests(uint64_t total_requests) {
			return this->total_requests += total_requests;
		}

		inline uint64_t GetFailedRequests() const {
			return failed_requests;
		}

		inline void SetFailedRequests(uint64_t failed_requests) {
			this->failed_requests = failed_requests;
		}

		inline uint64_t AddFailedRequests(uint64_t failed_requests) {
			return this->failed_requests += failed_requests;
		}

		inline int GetConcurrentRequests() const {
			return concurrent_requests;
		}

		inline void SetConcurrentRequests(int concurrent_requests) {
			this->concurrent_requests = concurrent_requests;
		}

		inline int AddConcurrentRequests(int concurrent_requests) {
			return this->concurrent_requests += concurrent_requests;
		}

		//data
		inline uint64_t GetBytesSent() const {
			return bytes_sent;
		}

		inline void SetBytesSent(uint64_t bytes_sent) {
			this->bytes_sent = bytes_sent;
		}

		inline uint64_t AddBytesSent(uint64_t bytes_sent) {
			return this->bytes_sent += bytes_sent;
		}

		inline uint64_t GetBytesReceived() const {
			return bytes_received;
		}

		inline void SetBytesReceived(uint64_t bytes_received) {
			this->bytes_received = bytes_received;
		}

		inline uint64_t AddBytesReceived(uint64_t bytes_received) {
			return this->bytes_received += bytes_received;
		}
};

}

#endif
