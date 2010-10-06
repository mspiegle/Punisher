/*
 * Logging
 * by Michael Spiegle
 * 09.08.09
 *
 * Logging.h
 */

#ifndef _LOGGING_H_
#define _LOGGING_H_

#include "String.h"

#include <vector>
#include <stdarg.h>

namespace Logging {

static const size_t max_frames = 64;
static const int frame_position = 3;

typedef enum {
	LEVEL_DEBUG,
	LEVEL_INFO,
	LEVEL_WARN,
	LEVEL_ERROR,
	LEVEL_CRITICAL
} logging_level_t;

class Level {
	private:
		logging_level_t level;

	public:
		Level();
		Level(logging_level_t level);
		~Level();

		inline logging_level_t Get() const {
			return level;
		}
		
		inline bool operator<(const Level& rhs) const {
			return level < rhs.level;
		}

		inline bool operator>(const Level& rhs) const {
			return level > rhs.level;
		}
		
		M::String ToString() const;
};

class StackFrame {
	public:
		int         frame;
		void*       pointer;
		M::String function;

		StackFrame();
		StackFrame(int frame, void* pointer, const M::String& function);
		~StackFrame();

		M::String ToString() const;
};

void SetLogLevel(Level level);
Level GetLogLevel();
void SetTrace(bool trace);
bool GetTrace();
void VLog(Level level, const char* message, va_list val);
void Log(Level level, const char* message, ...);

std::vector<StackFrame> StackDump();
M::String DemangleSymbol(const char* symbol);

void Debug(const char* message, ...);
void Info(const char* message, ...);
void Warn(const char* message, ...);
void Error(const char* message, ...);
void Critical(const char* message, ...);

//These are the MACRO versions of our logging functions.  We can completely
//turn off any logging function at compile time to optimize performance
#if defined DEBUG
#define LOGGING_DEBUG(x, ...) Logging::Debug(x, ## __VA_ARGS__)
#else
#define LOGGING_DEBUG(x, ...) 
#endif

}

#endif

