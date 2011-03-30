/*
 * Punisher
 * by Michael Spiegle
 * 10.12.09
 *
 * Md5Validator.cxx
 */

#include "Validator.hxx"
#include "Md5Validator.hxx"
#include "String.hxx"

#include <string.h>
#include <stdio.h>

namespace Punisher {

Md5Validator::Md5Validator() {
}

Md5Validator::Md5Validator(const char* hash) {
	Md5Validator::Assign(hash);
}

Md5Validator::Md5Validator(const M::String& hash) {
	Md5Validator::Assign(hash);
}

Md5Validator::~Md5Validator() {
}

void
Md5Validator::Assign(const char* hash) {
	this->hash.Assign(hash);
}

void
Md5Validator::Assign(const M::String& hash) {
	this->hash.Assign(hash);
}

void
Md5Validator::Update(void* buffer, size_t size) {
	this->hash.Update(buffer, size);
}

void
Md5Validator::Final() {
	this->hash.Final();
}

void
Md5Validator::GetBuffer(const void** buffer, size_t* size) const {
	*buffer = this->hash.ToBinary();
	*size = 16;
}

bool
Md5Validator::Compare(const Validator* validator) {
	//verify we have a validator, and compare
	if (NULL != validator) {
		const void* buffer = NULL;
		size_t size = 0;
		validator->GetBuffer(&buffer, &size);
		if (buffer != NULL && size == 16) {
			if (0 == memcmp(this->hash.ToBinary(), buffer, 16)) {
				return true;
			}
		}
	}

	return false;
}

Md5Validator*
Md5Validator::CreateValidator() const {
	return new Md5Validator();
}

M::String
Md5Validator::ToString() const {
	return hash.ToString();
}

} // Punisher::
