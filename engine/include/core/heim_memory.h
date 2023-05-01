#ifndef HEIM_MEMORY_H
#define HEIM_MEMORY_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
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
typedef struct {
	HEIM_MEMORY_TYPE type_counts[HEIM_MEMORY_TYPE_COUNT];
	size_t total_size;
	size_t total_allocations;
	size_t total_reallocations;
	size_t total_freed;
	HeimLogger *logger;
} HeimMemory;

/// @brief Creates a new HeimMemory struct.
/// @param logger Pointer to the logger.
/// @return Pointer to the new HeimMemory struct.
HeimMemory *heim_memory_create(HeimLogger *logger);

/// @brief Frees the HeimMemory struct.
/// @param memory Pointer to the HeimMemory struct.
void heim_memory_free(HeimMemory *memory);

// Memory allocation
void *heim_malloc(HeimMemory *memory, size_t size, HEIM_MEMORY_TYPE type);
void *heim_calloc(HeimMemory *memory, size_t nmemb, size_t size, HEIM_MEMORY_TYPE type);
void *heim_realloc(HeimMemory *memory, void *ptr, size_t size, HEIM_MEMORY_TYPE type);
void heim_free(HeimMemory *memory, void *ptr, HEIM_MEMORY_TYPE type);

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
#define HEIM_MALLOC(memory, type, heim_type) (type *)heim_malloc(memory, sizeof(type), heim_type)
/// @brief Wrapper for calloc.
#define HEIM_CALLOC(memory, type, nmemb, heim_type) (type *)heim_calloc(memory, nmemb, sizeof(type), heim_type)
/// @brief Wrapper for realloc.
#define HEIM_REALLOC(memory, type, ptr, heim_type) (type *)heim_realloc(memory, ptr, sizeof(type), heim_type)
/// @brief Wrapper for free.
#define HEIM_FREE(memory, ptr, heim_type) heim_free(memory, ptr, heim_type)

#endif // HEIM_MEMORY_H
