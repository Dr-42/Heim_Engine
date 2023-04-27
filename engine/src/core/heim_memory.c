#include "core/heim_memory.h"

#include <memory.h>

void *heim_malloc(size_t size){
		return malloc(size);
}
void *heim_calloc(size_t nmemb, size_t size){
		return calloc(nmemb, size);
}
void *heim_realloc(void *ptr, size_t size){
		return realloc(ptr, size);
}
void heim_free(void *ptr){
		free(ptr);
}

// Memory copy
void *heim_memcpy(void *dest, const void *src, size_t n){
		return memcpy(dest, src, n);
}
void *heim_memmove(void *dest, const void *src, size_t n){
		return memmove(dest, src, n);
}
void *heim_memset(void *s, int c, size_t n){
		return memset(s, c, n);
}
