/*
 * Md5Hash
 * by Michael Spiegle
 * 09.04.09
 *
 * Md5Hash.cpp
 */

#include "Md5Hash.h"
#include "String.h"

#include <string.h>
#include <openssl/md5.h>

namespace M {

const unsigned char
Md5Hash::ABTable[] = {
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //8
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //16
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //24
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //32
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //40
                       0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, //48
                       0x08, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //56
                       0x00, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x00, //64
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //72
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //80
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //88
                       0x00, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x00  //96
};


Md5Hash::Md5Hash() {
	Init();
}

//init ourselves with a hash passed as 16 binary bytes
Md5Hash::Md5Hash(const unsigned char arg[]) {
	Init();
	Assign(arg);
}

//init ourselves with a hash passed as a string
Md5Hash::Md5Hash(const char* arg) {
	Init();
	Assign(arg);
}

//init ourselves with a hash passed as a M::String
Md5Hash::Md5Hash(const M::String& arg) {
	Init();
	Assign(arg);
}

//init ourselves with an existing Md5Hash object
Md5Hash::Md5Hash(const Md5Hash& arg) {
	Init();
	Assign(arg);
}

Md5Hash&
Md5Hash::Assign(const unsigned char arg[]) {
	memcpy(hash, arg, sizeof(hash));
	return *this;
}

//use a lookup table to perform a fast string->binary conversion
Md5Hash&
Md5Hash::Assign(const char* arg) {
	size_t x;
	const char* ptr = arg;
	for (x = 0; x < sizeof(hash); x++) {
		//cast to int to quiet gcc
		hash[x] = (ABTable[(int)*ptr++] << 4) & 0xf0;
		hash[x] += ABTable[(int)*ptr++] & 0x0f;
	}
	return *this;
}

Md5Hash&
Md5Hash::Assign(const M::String& arg) {
	return Assign(arg.ToCString());
}

Md5Hash&
Md5Hash::Assign(const Md5Hash& arg) {
	memcpy(hash, arg.ToBinary(), sizeof(hash));
	return *this;
}

Md5Hash&
Md5Hash::operator=(const unsigned char hash[]) {
	return Assign(hash);
}

Md5Hash&
Md5Hash::operator=(const char* hash) {
	return Assign(hash);
}

Md5Hash&
Md5Hash::operator=(const M::String& hash) {
	return Assign(hash);
}

Md5Hash&
Md5Hash::operator=(const Md5Hash& hash) {
	return Assign(hash);
}

bool
Md5Hash::Compare(const Md5Hash* hash) {
	if (0 == memcmp(this->hash, hash->ToBinary(), 16)) {
		return true;
	}
	return false;
}

const unsigned char*
Md5Hash::ToBinary() const {
	return hash;
}

Md5Hash::~Md5Hash() {
}

void
Md5Hash::Init() {
	Clear();
	MD5_Init(&ctx);
}

void
Md5Hash::Clear() {
	memset(hash, 0, sizeof(hash));
}

void
Md5Hash::Update(const void* arg, size_t size) {
	MD5_Update(&ctx, (unsigned char*)arg, size);
}

void
Md5Hash::Final() {
	MD5_Final(hash, &ctx);
}

void
Md5Hash::Compute(const void* arg, size_t size) {
	MD5_Update(&ctx, (unsigned char*)arg, size);
	MD5_Final(hash, &ctx);
}

void
Md5Hash::Compute(const char* arg) {
	//compute hash for a null-terminated string
	Compute((const void*)arg, strlen(arg));
}

void
Md5Hash::Compute(const M::String& arg) {
	Compute(arg.ToCString());
}

M::String
Md5Hash::ToString() const {
	//fast method for converting from binary MD5 to ASCII
	char temp[33];
	char* ptr = temp;

	size_t x;
	for (x = 0; x < sizeof(hash); x++) {
		*ptr++ = "0123456789abcdef"[(hash[x] >> 4) & 0x0f];
		*ptr++ = "0123456789abcdef"[(hash[x] & 0x0f)];
	}
	*ptr = '\0';

	return M::String(temp);
}

}
