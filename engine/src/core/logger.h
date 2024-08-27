#pragma once 

#include "defines.h"

#define LOG_WARN_ENABLED 1
#define LOG_INFO_ENABLED 1
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1

#if VERELEASE == 1
#define LOG_DEBUG_ENABLED 0
#define LOG_TRACE_ENABLED 0
#endif

typedef enum log_level {
    LOG_LEVEL_FATAL = 0,
    LOG_LEVEL_ERROR = 1,
    LOG_LEVEL_WARN = 2,
    LOG_LEVEL_INFO = 3,
    LOG_LEVEL_DEBUG = 4,
    LOG_LEVEL_TRACE = 5,
} log_level;


/**
 * @brief Initializes logging system. Call twice; once with state = 0 to get required memory size,
 * then a second time passing allocated memory to state.
 * 
 * @param memory_requirement A pointer to hold the required memory size of internal state.
 * @param state 0 if just requesting memory requirement, otherwise allocated block of memory.
 * @returns b8 True on success otherwise false.
 */
b8 initialize_logging(u64* memory_requirement, void* state);
void shutdown_logging();

VEAPI void log_output(log_level level, const char* message, ...);

// Logs a fatal-level message
#define VEFATAL(message, ...) log_output(LOG_LEVEL_FATAL, message, ##__VA_ARGS__);

#ifndef VEERROR
// Logs an error-level message
#define VEERROR(message, ...) log_output(LOG_LEVEL_ERROR, message, ##__VA_ARGS__);
#endif

#if LOG_WARN_ENABLED == 1
// Logs a warning-level message
#define VEWARN(message, ...) log_output(LOG_LEVEL_WARN, message, ##__VA_ARGS__);
#else
// Does nothing when LOG_WARN_ENABLED != 1
#define VERWARN(message, ...)
#endif

#if LOG_INFO_ENABLED == 1
// Logs a info-level message
#define VEINFO(message, ...) log_output(LOG_LEVEL_INFO, message, ##__VA_ARGS__);
#else
// Does nothing when LOG_WARN_ENABLED != 1
#define VEINFO(message, ...)
#endif

#if LOG_DEBUG_ENABLED == 1
// Logs a debug-level message
#define VEDEBUG(message, ...) log_output(LOG_LEVEL_DEBUG, message, ##__VA_ARGS__);
#else
// Does nothing when LOG_WARN_ENABLED != 1
#define VEDEBUG(message, ...)
#endif

#if LOG_TRACE_ENABLED == 1
// Logs a warning-level message
#define VETRACE(message, ...) log_output(LOG_LEVEL_TRACE, message, ##__VA_ARGS__);
#else
// Does nothing when LOG_WARN_ENABLED != 1
#define VETRACE(message, ...)
#endif