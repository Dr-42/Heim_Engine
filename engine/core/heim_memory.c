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

typedef struct AllocationInfo {
    void *ptr;                    // Pointer to the allocated memory
    const char *file;             // File where allocation happened
    int line;                     // Line where allocation happened
    struct AllocationInfo *next;  // Pointer to the next allocation info
} AllocationInfo;

static AllocationInfo *head = NULL;

void heim_memory_close() {
    if (memory.total_allocations != memory.total_freed) {
        HEIM_LOG_WARN("Memory leak detected! %d allocations, %d freed", memory.total_allocations, memory.total_freed);
        HEIM_LOG_WARN("Total size: %d", memory.total_size);
        for (int i = 0; i < HEIM_MEMORY_TYPE_COUNT; i++) {
            HEIM_LOG_WARN("Type %s: %d", memory_type_names[i], memory.type_counts[i]);
        }

        // Print information about unfreed allocations
        AllocationInfo *current = head;
        while (current != NULL) {
            HEIM_LOG_WARN("Unfreed allocation: pointer %p, file %s, line %d", current->ptr, current->file, current->line);
            current = current->next;
        }
    } else {
        HEIM_LOG_INFO("No memory leaks detected");
    }
}

void *heim_malloc(size_t size, HEIM_MEMORY_TYPE type, const char *file, int line) {
    memory.type_counts[type]++;
    memory.total_size += size;
    memory.total_allocations++;

    void *ptr = malloc(size);

    // Create a new AllocationInfo
    AllocationInfo *info = (AllocationInfo *)malloc(sizeof(AllocationInfo));
    info->ptr = ptr;
    info->file = file;
    info->line = line;

    // Add the new AllocationInfo to the start of the list
    info->next = head;
    head = info;

    return ptr;
}

void *heim_calloc(size_t nmemb, size_t size, HEIM_MEMORY_TYPE type, const char *file, int line) {
    memory.type_counts[type]++;
    memory.total_size += size * nmemb;
    memory.total_allocations++;

    void *ptr = calloc(nmemb, size);

    // Create a new AllocationInfo
    AllocationInfo *info = (AllocationInfo *)malloc(sizeof(AllocationInfo));
    info->ptr = ptr;
    info->file = file;
    info->line = line;

    // Add the new AllocationInfo to the start of the list
    info->next = head;
    head = info;

    return ptr;
}

void *heim_realloc(void *ptr, size_t size, HEIM_MEMORY_TYPE type, const char *file, int line) {
    memory.type_counts[type]++;
    memory.total_size += size;
    memory.total_reallocations++;

    // Find and update the AllocationInfo in the list
    AllocationInfo *current = head;
    while (current != NULL) {
        if (current->ptr == ptr) {
            current->file = file;
            current->line = line;
            break;
        }
        current = current->next;
    }

    void *new_ptr = realloc(ptr, size);

    // If the memory location changed, update the ptr in the AllocationInfo
    if (new_ptr != ptr) {
        current->ptr = new_ptr;
    }

    return new_ptr;
}

void heim_free(void *ptr, HEIM_MEMORY_TYPE type) {
    memory.total_freed++;
    memory.type_counts[type]--;

    // Find and remove the AllocationInfo from the list
    AllocationInfo *current = head;
    AllocationInfo *previous = NULL;
    while (current != NULL) {
        if (current->ptr == ptr) {
            if (previous == NULL) {
                // It's the first element in the list
                head = current->next;
            } else {
                // It's not the first element
                previous->next = current->next;
            }
            free(current);
            break;
        }
        previous = current;
        current = current->next;
    }

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
