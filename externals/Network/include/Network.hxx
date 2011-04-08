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
	NETWORK_ERROR = 0,
	NETWORK_SUCCESS,
	NETWORK_WOULDBLOCK
} network_error_t;

}

#endif
