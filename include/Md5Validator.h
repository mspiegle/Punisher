/*
 * Punisher
 * by Michael Spiegle
 * 10.12.09
 *
 * Md5Validator.h
 *
 */

#ifndef _PUNISHER_MD5VALIDATOR_H_
#define _PUNISHER_MD5VALIDATOR_H_

#include "Validator.h"
#include "Md5Hash.h"
#include "String.h"

namespace Punisher {

class Md5Validator : public Validator {
	private:
		M::Md5Hash hash;

	public:
		Md5Validator();
		Md5Validator(const M::String& hash);
		Md5Validator(const char* hash);
		~Md5Validator();

		void Assign(const char* hash);
		void Assign(const M::String& hash);
		void Update(void* buffer, size_t size);
		void Final();
		bool Compare(const Validator* validator);
		void GetBuffer(const void** buffer, size_t* size) const;
		Md5Validator* CreateValidator() const;

		M::String ToString() const;
};

}

#endif
