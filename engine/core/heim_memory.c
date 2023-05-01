#include "core/heim_memory.h"
#include <memory.h>


HeimMemory *heim_memory_create(HeimLogger *logger){
	HeimMemory *memory = malloc(sizeof(HeimMemory));
	memory->total_size = 0;
	memory->total_allocations = 0;
	memory->total_reallocations = 0;
	memory->total_freed = 0;
	memset(memory->type_counts, 0, sizeof(memory->type_counts));
	memory->logger = logger;
	return memory;
}

void heim_memory_free(HeimMemory *memory){
	if(memory->total_allocations != memory->total_freed){
		HEIM_LOG_WARN(memory->logger, "Memory leak detected! %d allocations, %d freed", memory->total_allocations, memory->total_freed);
		HEIM_LOG_WARN(memory->logger, "Total size: %d", memory->total_size);
		for(int i = 0; i < HEIM_MEMORY_TYPE_COUNT; i++){
			HEIM_LOG_WARN(memory->logger, "Type %d: %d", i, memory->type_counts[i]);
		}
	} else {
		HEIM_LOG_INFO(memory->logger, "No memory leaks detected");
	}
	free(memory);
}

void *heim_malloc(HeimMemory *memory, size_t size, HEIM_MEMORY_TYPE type){
	memory->type_counts[type]++;
	memory->total_size += size;
	memory->total_allocations++;
	return malloc(size);
}

void *heim_calloc(HeimMemory *memory, size_t nmemb, size_t size, HEIM_MEMORY_TYPE type){
	memory->type_counts[type]++;
	memory->total_size += size * nmemb;
	memory->total_allocations++;
	return calloc(nmemb, size);
}

void *heim_realloc(HeimMemory *memory, void *ptr, size_t size, HEIM_MEMORY_TYPE type){
	memory->type_counts[type]++;
	memory->total_size += size;
	memory->total_reallocations++;
	return realloc(ptr, size);
}

void heim_free(HeimMemory *memory, void *ptr, HEIM_MEMORY_TYPE type){
	memory->total_freed++;
	memory->type_counts[type]--;
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
