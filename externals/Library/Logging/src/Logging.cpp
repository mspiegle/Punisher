/*
 * Logging
 * by Michael Spiegle
 * 09.08.09
 *
 * Logging.cpp
 */

#include "Logging.h"

#include <vector>
#include <string.h>
#include <time.h>
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <cxxabi.h>
#include <stdarg.h>

namespace Logging {

static Level current_level = LEVEL_ERROR;
static bool trace = false;

Level::Level() { }
Level::Level(logging_level_t level) {
	this->level = level;
}
Level::~Level() { }

M::String
Level::ToString() const {
	switch(level) {
		case LEVEL_DEBUG:
			return "Debug";
			break;
		case LEVEL_INFO:
			return "Info";
			break;
		case LEVEL_WARN:
			return "Warn";
			break;
		case LEVEL_ERROR:
			return "Error";
			break;
		case LEVEL_CRITICAL:
			return "Critical";
			break;
	}
	return "None";
}

void
SetLogLevel(Level level) {
	current_level = level;
}

void
SetTrace(bool trace) {
	Logging::trace = trace;
}

bool
GetTrace() {
	return trace;
}

Level
GetLogLevel() {
	return current_level;
}

void
VLog(Level level, const char* message, va_list args) {
	//get time/date
	time_t now;
	now = time(NULL);
	struct tm* local_now;
	local_now = localtime(&now);
	char date_string[128];
	if (0 == strftime(date_string, sizeof(date_string), "%D %T", local_now)) {
		strcpy(date_string, "Unknown");
	}

	//write log with tracing if desired
	char log_message[1024];
	vsnprintf(log_message, sizeof(log_message), message, args);
	if (trace) {
		std::vector<StackFrame> temp = StackDump();
		std::vector<StackFrame>::const_iterator i;
		M::String inline_stack;
		for (i = temp.begin(); i != temp.end(); ++i) {
			inline_stack += i->function;
			inline_stack += " <- ";
		}
		printf("[%s] [%s]: (%s) %s\n", 
		       date_string, level.ToString().ToCString(), log_message,
		       inline_stack.ToCString());
	} else {
		printf("[%s] [%s]: %s\n", 
		       date_string, level.ToString().ToCString(), log_message);
	}
}

void
Log(Level level, const char* message, ...) {
	va_list args;
	va_start(args, message);
	VLog(level, message, args);
	va_end(args);
}

void
Debug(const char* message, ...) {
	//only log desired messages
	if (LEVEL_DEBUG < current_level.Get()) {
		return;
	}

	va_list args;
	va_start(args, message);
	VLog(LEVEL_DEBUG, message, args);
	va_end(args);
}

void
Info(const char* message, ...) {
	//only log desired messages
	if (LEVEL_INFO < current_level.Get()) {
		return;
	}

	va_list args;
	va_start(args, message);
	VLog(LEVEL_INFO, message, args);
	va_end(args);
}

void
Warn(const char* message, ...) {
	//only log desired messages
	if (LEVEL_WARN < current_level.Get()) {
		return;
	}

	va_list args;
	va_start(args, message);
	VLog(LEVEL_WARN, message, args);
	va_end(args);
}

void
Error(const char* message, ...) {
	//only log desired messages
	if (LEVEL_ERROR < current_level.Get()) {
		return;
	}

	va_list args;
	va_start(args, message);
	VLog(LEVEL_ERROR, message, args);
	va_end(args);
}

void
Critical(const char* message, ...) {
	//only log desired messages
	if (LEVEL_CRITICAL < current_level.Get()) {
		return;
	}

	va_list args;
	va_start(args, message);
	VLog(LEVEL_CRITICAL, message, args);
	va_end(args);
}

StackFrame::StackFrame() {
	frame = 0;
	pointer = NULL;
	function = "Unknown";
}

StackFrame::StackFrame(int frame, void* pointer, const M::String& function) {
	this->frame = frame;
	this->pointer = pointer;
	this->function = function;
}

StackFrame::~StackFrame() { }

M::String
StackFrame::ToString() const {
	char temp[128];
	snprintf(temp, sizeof(temp), "%d: [%p] %s",
	         frame, pointer, function.ToCString());
	return temp;
}

std::vector<StackFrame>
StackDump() {
	//get our symbols, but skip this function
	void* addresses[max_frames];
	int size;
	size = backtrace(addresses, max_frames);
	size -= frame_position;
	char** symbols = backtrace_symbols(addresses + frame_position, size);

	//stick them in a vector and return them
	std::vector<StackFrame> temp;
	temp.reserve(size);
	int x;
	for (x = 0; x < size; ++x) {
		StackFrame frame(x, addresses[x], DemangleSymbol(symbols[x]).ToCString());
		temp.push_back(frame);
	}
	free(symbols);
	return temp;
}


M::String
DemangleSymbol(const char* symbol) {
	size_t size;
	int status;
	char temp[128];
	char* result;
	//first, try to demangle a c++ name
	if (1 == sscanf(symbol, "%*[^'(']%*[^'_']%[^')''+']", temp)) {
		if (NULL != (result = abi::__cxa_demangle(temp, NULL, &size, &status))) {
			return result;
		}
	}
	//if that didn't work, try to get a regular c symbol
	if (1 == sscanf(symbol, "%127s", temp)) {
		return temp;
	}

	//if all else fails, just return the symbol
	return symbol;
}

}
