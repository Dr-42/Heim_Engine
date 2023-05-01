#ifndef HEIM_LOGGER_H
#define HEIM_LOGGER_H

#include <stdio.h>

/// @brief Log levels.
typedef enum {
	HEIM_LOG_LEVEL_DEBUG,
	HEIM_LOG_LEVEL_INFO,
	HEIM_LOG_LEVEL_LOG,
	HEIM_LOG_LEVEL_WARN,
	HEIM_LOG_LEVEL_ERROR,
} HEIM_LOG_LEVEL;

/// @brief HeimLogger is a struct that contains all the information needed to log.
typedef struct {
	HEIM_LOG_LEVEL level;
	FILE *file;
} HeimLogger;

/// @brief Creates a new HeimLogger struct.
/// @param level Preferred log level.
/// @param file Output file. (can be stdout)
/// @return Pointer to the new HeimLogger struct.
HeimLogger *heim_logger_create(HEIM_LOG_LEVEL level, FILE *file);

void heim_log(HeimLogger *logger, HEIM_LOG_LEVEL level, const char *fmt, ...);

/// @brief Frees the HeimLogger struct.
/// @param logger Pointer to the HeimLogger struct.
void heim_logger_free(HeimLogger *logger);

/// @brief Log a message with the debug level.
#define HEIM_LOG_DEBUG(logger, fmt, ...) heim_log(logger, HEIM_LOG_LEVEL_DEBUG, fmt, ##__VA_ARGS__)
/// @brief Log a message with the info level.
#define HEIM_LOG_INFO(logger, fmt, ...) heim_log(logger, HEIM_LOG_LEVEL_INFO, fmt, ##__VA_ARGS__)
/// @brief Log a message with the log level.
#define HEIM_LOG_LOG(logger, fmt, ...) heim_log(logger, HEIM_LOG_LEVEL_LOG, fmt, ##__VA_ARGS__)
/// @brief Log a message with the warn level.
#define HEIM_LOG_WARN(logger, fmt, ...) heim_log(logger, HEIM_LOG_LEVEL_WARN, fmt, ##__VA_ARGS__)
/// @brief Log a message with the error level.
#define HEIM_LOG_ERROR(logger, fmt, ...) heim_log(logger, HEIM_LOG_LEVEL_ERROR, fmt, ##__VA_ARGS__)

#endif
