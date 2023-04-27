#include "core/heim_logger.h"
#include <stdarg.h>
#include <stdlib.h>

HeimLogger *heim_logger_create(HEIM_LOG_LEVEL level, FILE *file){
	HeimLogger *logger = malloc(sizeof(HeimLogger));
	logger->level = level;
	logger->file = file;
	return logger;
}

#define YELLOW "\033[1;33m"
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define RESET "\033[0m"

void heim_log(HeimLogger *logger, HEIM_LOG_LEVEL level, const char *fmt, ...){
	if (level >= logger->level) {
		switch (level) {
			case HEIM_LOG_LEVEL_DEBUG:
				fprintf(logger->file, "%s[DEBUG] %s", MAGENTA, RESET);
				break;
			case HEIM_LOG_LEVEL_INFO:
				fprintf(logger->file, "%s[INFO] %s", BLUE, RESET);
				break;
			case HEIM_LOG_LEVEL_LOG:
				fprintf(logger->file, "%s[LOG] %s", GREEN, RESET);
				break;
			case HEIM_LOG_LEVEL_WARN:
				fprintf(logger->file, "%s[WARN] %s", YELLOW, RESET);
				break;
			case HEIM_LOG_LEVEL_ERROR:
				fprintf(logger->file, "%s[ERROR] %s", RED, RESET);
				break;
			default:
				break;
		}
		va_list args;
		va_start(args, fmt);
		vfprintf(logger->file, fmt, args);
		va_end(args);
		fprintf(logger->file, "\n");
	}
}

void heim_logger_free(HeimLogger *logger){
	free(logger);
}
