#ifndef HEIM_MEMORY_H
#define HEIM_MEMORY_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

// Memory allocation
void *heim_malloc(size_t size);
void *heim_calloc(size_t nmemb, size_t size);
void *heim_realloc(void *ptr, size_t size);
void heim_free(void *ptr);

// Memory copy
void *heim_memcpy(void *dest, const void *src, size_t n);
void *heim_memmove(void *dest, const void *src, size_t n);
void *heim_memset(void *s, int c, size_t n);

#define heim_memzero(s, n) heim_memset(s, 0, n)

#define HEIM_MALLOC(type) (type *)heim_malloc(sizeof(type))
#define HEIM_CALLOC(nmemb, type) (type *)heim_calloc(nmemb, sizeof(type))
#define HEIM_REALLOC(ptr, type) (type *)heim_realloc(ptr, sizeof(type))
#define HEIM_FREE(ptr) heim_free(ptr)

#endif // HEIM_MEMORY_H
