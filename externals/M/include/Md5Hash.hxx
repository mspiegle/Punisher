/*
 * Md5Hash
 * by Michael Spiegle
 * 09.04.09
 *
 * Md5Hash.h
 */

#ifndef _M_MD5HASH_H_
#define _M_MD5HASH_H_

#include "String.hxx"

#include <openssl/md5.h>

namespace M {

class Md5Hash {
	private:
		unsigned char              hash[16];
		MD5_CTX                    ctx;
		static const unsigned char ABTable[];
	
	public:
		Md5Hash();
		~Md5Hash();

		void Init();
		void Clear();
		void Final();
		M::String            ToString() const;
		const unsigned char* ToBinary() const;

		//these functions work with the hash itself
		Md5Hash(const unsigned char[]);
		Md5Hash(const char*);
		Md5Hash(const M::String&);
		Md5Hash(const Md5Hash&);
		Md5Hash& Assign(const unsigned char hash[]);
		Md5Hash& Assign(const char* hash);
		Md5Hash& Assign(const M::String&);
		Md5Hash& Assign(const Md5Hash&);
		Md5Hash& operator=(const unsigned char hash[]);
		Md5Hash& operator=(const char* hash);
		Md5Hash& operator=(const M::String& hash);
		Md5Hash& operator=(const Md5Hash&);
		bool operator==(const Md5Hash& hash);
		bool operator==(const char* hash);
		bool Compare(const Md5Hash* hash);

		//these functions work with raw data (or strings)
		void Update(const void*, size_t);
		void Compute(const void*, size_t);
		void Compute(const char*);
		void Compute(const M::String&);
};

}

#endif
