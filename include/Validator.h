/*
 * Punisher
 * by Michael Spiegle
 * 10.12.09
 *
 * Validator.h
 *
 */

#ifndef _PUNISHER_VALIDATOR_H_
#define _PUNISHER_VALIDATOR_H_

#include "String.h"

#include <stddef.h>

namespace Punisher {

class Validator {
	public:
		Validator();
		virtual ~Validator();

		virtual void Update(void* buffer, size_t size) = 0;
		virtual void Final() = 0;
		virtual bool Compare(const Validator* validator) = 0;
		virtual void GetBuffer(const void** buffer, size_t* size) const = 0;
		virtual Validator* CreateValidator() const = 0;

		virtual M::String ToString() const = 0;
};

}

#endif
