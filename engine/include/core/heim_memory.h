#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "core/heim_logger.h"

/// @brief Type of memory allocation.
typedef enum {
    HEIM_MEMORY_TYPE_BASE,
    HEIM_MEMORY_TYPE_ECS,
    HEIM_MEMORY_TYPE_RENDERER,
    HEIM_MEMORY_TYPE_COUNT
} HEIM_MEMORY_TYPE;

/// @brief HeimMemory is a struct that contains all the information needed to manage memory.
typedef struct HeimMemory {
    HEIM_MEMORY_TYPE type_counts[HEIM_MEMORY_TYPE_COUNT];
    size_t total_size;
    size_t total_allocations;
    size_t total_reallocations;
    size_t total_freed;
} HeimMemory;

/// @brief Perform closing operations for the memory.
void heim_memory_close();

// Memory allocation
void *heim_malloc(size_t size, HEIM_MEMORY_TYPE type, const char *file, int line);
void *heim_calloc(size_t nmemb, size_t size, HEIM_MEMORY_TYPE type, const char *file, int line);
void *heim_realloc(void *ptr, size_t size, HEIM_MEMORY_TYPE type, const char *file, int line);
void heim_free(void *ptr, HEIM_MEMORY_TYPE type);

// Memory copy
/// @brief Wrapper for memcpy.
void *heim_memcpy(void *dest, const void *src, size_t n);
/// @brief Wrapper for memmove.
void *heim_memmove(void *dest, const void *src, size_t n);
/// @brief Wrapper for memset.
void *heim_memset(void *s, int c, size_t n);

/// @brief Zeroes out the memory.
#define heim_memzero(s, n) heim_memset(s, 0, n)

// Memory allocation macros

/// @brief Wrapper for malloc.
#define HEIM_MALLOC(type, heim_type) (type *)heim_malloc(sizeof(type), heim_type, __FILE__, __LINE__)
/// @brief Wrapper for calloc.
#define HEIM_CALLOC(type, nmemb, heim_type) (type *)heim_calloc(nmemb, sizeof(type), heim_type, __FILE__, __LINE__)
/// @brief Wrapper for realloc.
#define HEIM_REALLOC(type, ptr, heim_type) (type *)heim_realloc(ptr, sizeof(type), heim_type, __FILE__, __LINE__)
/// @brief Wrapper for free.
#define HEIM_FREE(ptr, heim_type) heim_free(ptr, heim_type)