/*
 * String.h
 *
 *  Created on: Oct 27, 2008
 *      Author: mspiegle
 */

#ifndef _M_STRING_H_
#define _M_STRING_H_

#ifndef M_STRING_DEFAULT_SIZE
#define M_STRING_DEFAULT_SIZE 64
#endif

#include <stddef.h>

namespace M {

class String {
	private:
		size_t capacity;
		size_t length;
		
		char*  data;
		char   local_data[M_STRING_DEFAULT_SIZE];

	public:
		String();
		String(const String& arg);
		String(const char* arg);
		String(char arg);
		~String();
		
		static const size_t npos = -1;

		size_t Capacity() const;
		size_t Size() const;
		size_t Length() const;
		void Reserve(size_t arg = 0);

		String& operator=(const String& arg);
		String& operator=(const char* arg);
		String& operator=(char c);

		bool operator<(const String& arg);
		bool operator<(const char* arg);

		bool operator>(const String& arg);
		bool operator>(const char* arg);

		inline String& operator+=(const char* arg) {
			return Append(arg);
		}

		inline String& operator+=(char arg) {
			return Append(arg);
		}

		inline String& operator+=(const String& arg) {
			return Append(arg);
		}

		bool operator==(const String& arg);
		bool operator==(const char* arg);

		bool operator!=(const String& arg);
		bool operator!=(const char* arg);

		const char& operator[](size_t pos) const;
		char& operator[](size_t pos);

		String& Append(const char* arg);

		inline String& Append(char c) {
			if ((capacity - length - 1) < 1) {
				Reserve(capacity + 1);
			}

			data[length] = c;
			++length;
			data[length] = '\0';
			return *this;
		}

		inline String& Append(unsigned int count, char c) {
			if ((capacity - length - 1) < count) {
				Reserve(capacity + count);
			}

			while (count > 0) {
				data[length] = c;
				++length;
				--count;
			}

			data[length] = '\0';

			return *this;
		}

		String& Append(const String& arg);

		String& Assign(const String& arg);
		String& Assign(const char* arg);
		String& Assign(char c);

		int Compare(const String& arg) const;
		int Compare(const char* arg) const;

		size_t Find(const String& arg, size_t pos = 0) const;
		size_t Find(const char* arg, size_t pos = 0) const;

		inline void Clear() {
			length = 0;
			data[0] = '\0';
		}

		String& Erase(size_t pos, size_t size);

		inline String& Erase(size_t pos) {
			if (pos < length) {
				data[pos] = '\0';
			}
			return *this;
		}

		inline const char* ToCString() const {
			return data;
		}

		String Substring(size_t start = 0, size_t size = npos) const;

		size_t FindFirst(const String& str, size_t pos = 0) const;
		size_t FindFirst(const char* s, size_t pos = 0) const;
};

inline bool
operator<(const String& first, const String& second) {
	return first.Compare(second) < 0;
}

inline bool
operator<(const String& first, const char* second) {
	return first.Compare(second) < 0;
}

inline bool
operator<(const char* first, const String& second) {
	return second.Compare(first) > 0;
}

inline bool
operator>(const String& first, const String& second) {
	return first.Compare(second) > 0;
}

inline bool
operator>(const String& first, const char* second) {
	return first.Compare(second) > 0;
}

inline bool
operator>(const char* first, const String& second) {
	return second.Compare(first) < 0;
}

inline bool
operator==(const String& first, const String& second) {
	return first.Compare(second) == 0;
}

inline bool
operator==(const String& first, const char* second) {
	return first.Compare(second) == 0;
}

inline bool
operator==(const char* first, const String& second) {
	return second.Compare(first) == 0;
}

inline String
operator+(const String& first, const String& second) {
	String temp(first);
	temp.Append(second);
	return temp;
}

inline String
operator+(const char* first, const String& second) {
	String temp(first);
	temp.Append(second);
	return temp;
}

inline String
operator+(char first, const String& second) {
	String temp(first);
	temp.Append(second);
	return temp;
}

inline String
operator+(const String& first, const char* second) {
	String temp(first);
	temp.Append(second);
	return temp;
}

inline String
operator+(const String& first, char second) {
	String temp(first);
	temp.Append(second);
	return temp;
}

struct Hash {
	size_t operator()(const String& arg) const {
		size_t hash = 0;
		const char* data = arg.ToCString();
		while (*data) {
			hash = hash * 101 + *data++;
		}
		return hash;
	}
};

} //M::

#endif /* _MSTRING_H_ */
