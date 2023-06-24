#include "core/heim_memory.h"

#include <memory.h>
#include <string.h>

static HeimMemory memory = {
    .total_size = 0,
    .total_allocations = 0,
    .total_reallocations = 0,
    .total_freed = 0,
    .type_counts = {0},
};

static const char *memory_type_names[] = {
    "Base",
    "ECS",
    "Renderer",
};

void heim_memory_close() {
    if (memory.total_allocations != memory.total_freed) {
        HEIM_LOG_WARN("Memory leak detected! %d allocations, %d freed", memory.total_allocations, memory.total_freed);
        HEIM_LOG_WARN("Total size: %d", memory.total_size);
        for (int i = 0; i < HEIM_MEMORY_TYPE_COUNT; i++) {
            HEIM_LOG_WARN("Type %s: %d", memory_type_names[i], memory.type_counts[i]);
        }
    } else {
        HEIM_LOG_INFO("No memory leaks detected");
    }
}

void *heim_malloc(size_t size, HEIM_MEMORY_TYPE type) {
    memory.type_counts[type]++;
    memory.total_size += size;
    memory.total_allocations++;
    return malloc(size);
}

void *heim_calloc(size_t nmemb, size_t size, HEIM_MEMORY_TYPE type) {
    memory.type_counts[type]++;
    memory.total_size += size * nmemb;
    memory.total_allocations++;
    return calloc(nmemb, size);
}

void *heim_realloc(void *ptr, size_t size, HEIM_MEMORY_TYPE type) {
    memory.type_counts[type]++;
    memory.total_size += size;
    memory.total_reallocations++;
    return realloc(ptr, size);
}

void heim_free(void *ptr, HEIM_MEMORY_TYPE type) {
    memory.total_freed++;
    memory.type_counts[type]--;
    free(ptr);
}

// Memory copy
void *heim_memcpy(void *dest, const void *src, size_t n) {
    return memcpy(dest, src, n);
}
void *heim_memmove(void *dest, const void *src, size_t n) {
    return memmove(dest, src, n);
}
void *heim_memset(void *s, int c, size_t n) {
    return memset(s, c, n);
}
