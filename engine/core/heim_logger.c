#include "core/heim_logger.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

static HeimLogger logger = (HeimLogger){
    .level = HEIM_LOG_LEVEL_INFO,
    .file = NULL,
};

#define UNI_RED "\033[0;31m"
#define UNI_GREEN "\033[0;32m"
#define UNI_YELLOW "\033[0;33m"
#define UNI_BLUE "\033[0;34m"
#define UNI_MAGENTA "\033[0;35m"
#define UNI_RESET "\033[0m"

static const char *log_level_names[] = {
    "DEBUG : ",
    "INFO : ",
    "LOG : ",
    "WARN : ",
    "ERROR : ",
};

void heim_log_init(const char *filename, HEIM_LOG_LEVEL level) {
    if (filename == NULL) {
        logger.file = stdout;
    } else {
        logger.file = fopen(filename, "w");
        if (logger.file == NULL) {
            fprintf(stderr, "Failed to open log file %s\n", filename);
            return;
        }
    }

    logger.level = level;
}
void heim_logger_close() {
    if (logger.file != NULL && logger.file != stdout) {
        fclose(logger.file);
    }
}

void heim_log(HEIM_LOG_LEVEL level, const char *fmt, ...) {
    if (level > logger.level) {
        return;
    }

    va_list args;
    va_start(args, fmt);

    char buffer[1024];

    vsnprintf(buffer, sizeof(buffer), fmt, args);
    if (logger.file != NULL && logger.file != stdout)
        fprintf(logger.file, "%s%s\n", log_level_names[level], buffer);
    else {
        switch (level) {
            case HEIM_LOG_LEVEL_ERROR:
                fprintf(stderr, "%s%s%s%s\n", UNI_RED, log_level_names[level], UNI_RESET, buffer);
                break;
            case HEIM_LOG_LEVEL_WARN:
                fprintf(stderr, "%s%s%s%s\n", UNI_YELLOW, log_level_names[level], UNI_RESET, buffer);
                break;
            case HEIM_LOG_LEVEL_LOG:
                fprintf(stderr, "%s%s%s%s\n", UNI_GREEN, log_level_names[level], UNI_RESET, buffer);
                break;
            case HEIM_LOG_LEVEL_INFO:
                fprintf(stderr, "%s%s%s%s\n", UNI_BLUE, log_level_names[level], UNI_RESET, buffer);
                break;
            case HEIM_LOG_LEVEL_DEBUG:
                fprintf(stderr, "%s%s%s%s\n", UNI_MAGENTA, log_level_names[level], UNI_RESET, buffer);
                break;
            default:
                break;
        }
    }
    va_end(args);
    fflush(logger.file);
}