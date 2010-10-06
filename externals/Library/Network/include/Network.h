/*
 * Network
 * by Michael Spiegle
 * 09.04.09
 *
 * Network.h
 */

#ifndef _NETWORK_NETWORK_H_
#define _NETWORK_NETWORK_H_

namespace Network {

typedef enum {
	NETWORK_ERROR = 0,
	NETWORK_SUCCESS,
	NETWORK_WOULDBLOCK
} network_error_t;

}

#endif
