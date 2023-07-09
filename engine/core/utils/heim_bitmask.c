#include "core/utils/heim_bitmask.h"

#include <assert.h>

#include "core/heim_logger.h"
#include "core/heim_memory.h"
#include "core/utils/heim_vector.h"

#define DATA_SIZE(x) x % 64 == 0 ? x / 64 : (x / 64) + 1

HeimBitmask* heim_bitmask_create(uint64_t size) {
    HeimBitmask* bitmask = HEIM_MALLOC(HeimBitmask, HEIM_MEMORY_TYPE_BASE);
    if (bitmask == NULL) {
        HEIM_LOG_ERROR("Failed to allocate memory for bitmask");
        return NULL;
    }
    heim_memzero(bitmask, sizeof(HeimBitmask));
    bitmask->size = size;
    uint64_t data_size = DATA_SIZE(size);
    bitmask->data = HEIM_CALLOC(uint64_t, data_size, HEIM_MEMORY_TYPE_BASE);
    return bitmask;
}
void heim_bitmask_destroy(HeimBitmask* bitmask) {
    if (bitmask == NULL) {
        HEIM_LOG_WARN("Bitmask is NULL");
        return;
    }
    HEIM_FREE(bitmask->data, HEIM_MEMORY_TYPE_BASE);
    HEIM_FREE(bitmask, HEIM_MEMORY_TYPE_BASE);
}

void heim_bitmask_set(HeimBitmask* bitmask, uint64_t index) {
    if (bitmask == NULL) {
        HEIM_LOG_WARN("Bitmask is NULL");
        return;
    }
    if (index >= bitmask->size) {
        HEIM_LOG_ERROR("Bitmask index out of range");
        return;
    }
    uint64_t data_index = index / 64;
    uint64_t bit_index = index % 64;
    bitmask->data[data_index] |= (1ULL << bit_index);
}
void heim_bitmask_unset(HeimBitmask* bitmask, uint64_t index) {
    if (bitmask == NULL) {
        HEIM_LOG_WARN("Bitmask is NULL");
        return;
    }
    if (index >= bitmask->size) {
        HEIM_LOG_ERROR("Bitmask index out of range");
        return;
    }
    uint64_t data_index = index / 64;
    uint64_t bit_index = index % 64;
    bitmask->data[data_index] &= ~(1ULL << bit_index);
}
bool heim_bitmask_get(HeimBitmask* bitmask, uint64_t index) {
    if (bitmask == NULL) {
        HEIM_LOG_WARN("Bitmask is NULL");
        return;
    }
    if (index >= bitmask->size) {
        HEIM_LOG_ERROR("Bitmask index out of range");
        return false;
    }
    uint64_t data_index = index / 64;
    uint64_t bit_index = index % 64;
    return (bitmask->data[data_index] & (1ULL << bit_index)) != 0;
}

void heim_bitmask_clear(HeimBitmask* bitmask) {
    if (bitmask == NULL) {
        HEIM_LOG_WARN("Bitmask is NULL");
        return;
    }
    uint64_t data_size = DATA_SIZE(bitmask->size);
    heim_memzero(bitmask->data, data_size * sizeof(uint64_t));
}

void heim_bitmask_resize(HeimBitmask* bitmask, uint64_t size) {
    if (bitmask == NULL) {
        HEIM_LOG_WARN("Bitmask is NULL");
        return;
    }
    uint64_t data_size = DATA_SIZE(size);
    if (data_size != DATA_SIZE(bitmask->size)) {
        uint64_t* new_data = HEIM_CALLOC(uint64_t, data_size, HEIM_MEMORY_TYPE_BASE);
        if (new_data == NULL) {
            HEIM_LOG_ERROR("Failed to allocate memory for bitmask");
            return;
        }
        uint64_t copy_size = data_size < DATA_SIZE(bitmask->size) ? data_size : DATA_SIZE(bitmask->size);
        heim_memcpy(new_data, bitmask->data, copy_size * sizeof(uint64_t));
        HEIM_FREE(bitmask->data, HEIM_MEMORY_TYPE_BASE);
        bitmask->data = new_data;
    }
    bitmask->size = size;
}

void heim_bitmask_copy(HeimBitmask* dest, HeimBitmask* src) {
    if (dest == NULL) {
        HEIM_LOG_WARN("Destination bitmask is NULL");
        return;
    }
    if (src == NULL) {
        HEIM_LOG_WARN("Source bitmask is NULL");
        return;
    }
    if (dest->size != src->size) {
        HEIM_LOG_ERROR("Bitmask sizes do not match");
        return;
    }
    uint64_t data_size = DATA_SIZE(dest->size);
    heim_memcpy(dest->data, src->data, data_size * sizeof(uint64_t));
}

void heim_bitmask_push_set(HeimBitmask* bitmask) {
    if (bitmask == NULL) {
        HEIM_LOG_WARN("Bitmask is NULL");
        return;
    }
    heim_bitmask_resize(bitmask, bitmask->size + 1);
    heim_bitmask_set(bitmask, bitmask->size - 1);
}