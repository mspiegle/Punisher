/*
 * Socket
 * by Michael Spiegle
 * 09.04.09
 *
 * Socket.h
 */

#ifndef _NETWORK_SOCKET_H_
#define _NETWORK_SOCKET_H_

#include "Network.h"

#include <sys/types.h>
#include <sys/socket.h>

namespace Network {

typedef enum {
	Unix = AF_UNIX,
	Inet = AF_INET
} Domain;

typedef enum {
	Raw      = SOCK_RAW,
	Stream   = SOCK_STREAM,
	Datagram = SOCK_DGRAM
} Type;

typedef enum {
	Default = 0
} Protocol;

class Socket {
	private:
		int   fd;
		void* notes;
		bool  listening;
		bool  connected;

	public:
		Socket();
		Socket(int fd);
		virtual ~Socket();
		void Init();

		int  GetFd() const;
		void SetFd(int fd);

		void* GetNotes() const;
		void  SetNotes(void* notes);

		network_error_t Read(void* buffer, size_t* size);
		network_error_t Write(const void* buffer, size_t* size);

		int GetStatusFlags() const;

		bool GetBlocking() const;
		bool SetBlocking(bool blocking);

		bool SetReuseAddr(bool reuse);

		bool Create(int domain, int type, int protocol);
		virtual bool Close();
		bool Listen(int backlog);
		bool Listening() const;
		Socket* Accept();

		network_error_t Connect(const struct sockaddr* addr, socklen_t size);
		bool GetConnected() const;

		network_error_t Bind(const struct sockaddr* addr, socklen_t size);
	};

}

#endif
