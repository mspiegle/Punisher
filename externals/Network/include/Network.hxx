/*
 * Network
 * by Michael Spiegle
 * 09.04.09
 *
 * Network.hxx
 */

#ifndef _NETWORK_NETWORK_HXX_
#define _NETWORK_NETWORK_HXX_

namespace Network {

typedef enum {
	NETWORK_SUCCESS = 0,
	NETWORK_WOULDBLOCK,
	NETWORK_ERROR
} network_error_t;

typedef enum {
	SOCKET_TCP = 0,
	SOCKET_UDP,
	SOCKET_UNIX
} socket_type_t;


}

#endif
