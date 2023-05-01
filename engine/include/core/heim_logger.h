#ifndef HEIM_LOGGER_H
#define HEIM_LOGGER_H

#include <stdio.h>

typedef enum {
	HEIM_LOG_LEVEL_DEBUG,
	HEIM_LOG_LEVEL_INFO,
	HEIM_LOG_LEVEL_LOG,
	HEIM_LOG_LEVEL_WARN,
	HEIM_LOG_LEVEL_ERROR,
} HEIM_LOG_LEVEL;

typedef struct {
	HEIM_LOG_LEVEL level;
	FILE *file;
} HeimLogger;

HeimLogger *heim_logger_create(HEIM_LOG_LEVEL level, FILE *file);
void heim_log(HeimLogger *logger, HEIM_LOG_LEVEL level, const char *fmt, ...);
void heim_logger_free(HeimLogger *logger);

#define HEIM_LOG_DEBUG(logger, fmt, ...) heim_log(logger, HEIM_LOG_LEVEL_DEBUG, fmt, ##__VA_ARGS__)
#define HEIM_LOG_INFO(logger, fmt, ...) heim_log(logger, HEIM_LOG_LEVEL_INFO, fmt, ##__VA_ARGS__)
#define HEIM_LOG_LOG(logger, fmt, ...) heim_log(logger, HEIM_LOG_LEVEL_LOG, fmt, ##__VA_ARGS__)
#define HEIM_LOG_WARN(logger, fmt, ...) heim_log(logger, HEIM_LOG_LEVEL_WARN, fmt, ##__VA_ARGS__)
#define HEIM_LOG_ERROR(logger, fmt, ...) heim_log(logger, HEIM_LOG_LEVEL_ERROR, fmt, ##__VA_ARGS__)

#endif
