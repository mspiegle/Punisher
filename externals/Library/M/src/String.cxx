/*
 * String.cpp
 *
 *  Created on: Oct 27, 2008
 *      Author: mspiegle
 */

#include "String.hxx"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

namespace M {

String::String() {
	capacity = M_STRING_DEFAULT_SIZE;
	length = 0;
	data = local_data;
	data[0] = '\0';
}

String::String(const String& arg) {
	if (arg.capacity > M_STRING_DEFAULT_SIZE) {
		capacity = arg.capacity;
		data = (char*)malloc(arg.capacity);
	}	else {
		capacity = M_STRING_DEFAULT_SIZE;
		data = local_data;
	}

	length = arg.length;
	memcpy(data, arg.data, length);
	data[length] = '\0';
}

String::String(const char* arg) {
	length = strlen(arg);

	if ((length + 1) > M_STRING_DEFAULT_SIZE) {
		capacity = length + 1;
		data = (char*)malloc(capacity);
	} else {
		capacity = M_STRING_DEFAULT_SIZE;
		data = local_data;
	}

	memcpy(data, arg, length);
	data[length] = '\0';
}

String::String(char arg) {
	capacity = M_STRING_DEFAULT_SIZE;
	length = 1;
	data[0] = arg;
	data[1] = '\0';
}

String::~String() {
	if (data != local_data) {
		free(data);
	}
}

size_t
String::Capacity() const {
	return capacity;
}

void
String::Reserve(size_t arg) {
	//this function works with raw bytes
	if (arg > capacity) {
		if (data == local_data) {
			data = (char*)malloc(arg);
			memcpy(data, local_data, length);
			data[length] = '\0';
		} else {
			data = (char*)realloc(data, arg);
		}
		capacity = arg;
	}
}

size_t
String::Size() const {
	return length;
}

size_t
String::Length() const {
	return length;
}

bool
String::operator<(const String& arg) {
	return (Compare(arg) < 0) ? true : false;
}

bool
String::operator<(const char* arg) {
	return (Compare(arg) < 0) ? true : false;
}

bool
String::operator>(const String& arg) {
	return (Compare(arg) > 0) ? true : false;
}

bool
String::operator>(const char* arg) {
	return (Compare(arg) > 0) ? true : false;
}

String&
String::operator=(const String& arg) {
	return Assign(arg);
}

String&
String::operator=(const char* arg) {
	return Assign(arg);
}

String&
String::operator=(char c) {
	return Assign(c);
}

bool
String::operator==(const String& arg) {
	return (Compare(arg) == 0) ? true : false;
}

bool
String::operator==(const char* arg) {
	return (Compare(arg) == 0) ? true : false;
}

bool
String::operator!=(const String& arg) {
	return (Compare(arg) != 0) ? true : false;
}

bool
String::operator!=(const char* arg) {
	return (Compare(arg) != 0) ? true : false;
}

const char&
String::operator[](size_t pos) const {
	return (const char&)data[pos];
}

char&
String::operator[](size_t pos) {
	return (char&)data[pos];
}

String&
String::Append(const char* arg) {
	size_t needed_capacity = strlen(arg) + 1;
	size_t available = capacity - length - 1;

	if (needed_capacity > available) {
		Reserve(capacity + (needed_capacity - available));
	}

	memcpy(data + length, arg, needed_capacity);

	length += needed_capacity - 1;
	data[length] = '\0';

	return *this;
}

String&
String::Append(const String& arg) {
	return Append(arg.data);
}

String&
String::Assign(const char* arg) {
	size_t needed_capacity = strlen(arg) + 1;

	if (needed_capacity > capacity) {
		Reserve(needed_capacity);
	}

	memcpy(data, arg, needed_capacity);
	
	length = needed_capacity - 1;
	data[length] = '\0';

	return *this;
}

String&
String::Assign(const String& arg) {
	return Assign(arg.data);
}

String&
String::Assign(char c) {
	length = 1;
	data[0] = c;
	data[1] = '\0';

	return *this;
}

int
String::Compare(const String& arg) const {
	return strcmp(data, arg.data);
}

int
String::Compare(const char* arg) const {
	return strcmp(data, arg);
}

size_t
String::Find(const String& arg, size_t offset) const {
	return Find(arg.data, offset);
}

size_t
String::Find(const char* arg, size_t offset) const {
	char* location = NULL;
	if (NULL != (location = strstr(data + offset, arg))) {
		size_t position = location - data;
		if (position <= length) {
			return position;
		}
	}
	return npos;
}

String
String::Substring(size_t start, size_t size) const {
	if (start > length) {
		return String();
	}

	String temp;
	size = (size > length) ? (length - start) : size;
	if ((size + start) > length) {
		return String();
	}
	temp.Reserve(size + 1);

	memcpy(temp.data, data + start, size);
	temp.data[size] = '\0';
	temp.length = size;
	return temp;
}

size_t
String::FindFirst(const char* search, size_t pos) const {
	size_t search_size = strlen(search);
	for (size_t data_iter = pos; data_iter < length; data_iter++) {
		for (size_t search_iter = 0; search_iter < search_size; search_iter++) {
			if (data[data_iter] == search[search_iter]) {
				return data_iter;
			}
		}
	}
	return npos;
}

size_t
String::FindFirst(const String& search, size_t pos) const {
	return FindFirst(search.data, pos);
}

String&
String::Erase(size_t pos = 0, size_t size = -1) {
	if (pos + size > length) {
		return *this;
	}

	memcpy(data + pos, data + pos + size, length - pos + size);
	length -= size;
	data[length] = '\0';
	return *this;
}

} //M::
