#include "core/utils/heim_bitmask.h"

#include <assert.h>

#include "core/heim_logger.h"
#include "core/heim_memory.h"
#include "core/utils/heim_vector.h"

HeimBitmask* heim_bitmask_create(uint64_t size) {
    assert(sizeof(uint64_t) == 8);
    HeimBitmask* bitmask = HEIM_MALLOC(HeimBitmask, HEIM_MEMORY_TYPE_BASE);
    if (bitmask == NULL) {
        HEIM_LOG_ERROR("Failed to allocate memory for bitmask");
        return NULL;
    }
    heim_memzero(bitmask, sizeof(HeimBitmask));
    bitmask->size = size;
    uint64_t data_size = (size / 64) + 1;
    bitmask->data = HEIM_CALLOC(uint64_t, data_size, HEIM_MEMORY_TYPE_BASE);
    return bitmask;
}
void heim_bitmask_destroy(HeimBitmask* bitmask) {
    HEIM_FREE(bitmask->data, HEIM_MEMORY_TYPE_BASE);
    HEIM_FREE(bitmask, HEIM_MEMORY_TYPE_BASE);
}

void heim_bitmask_set(HeimBitmask* bitmask, uint64_t index) {
    if (index >= bitmask->size) {
        HEIM_LOG_ERROR("Bitmask index out of range");
        return;
    }
    uint64_t data_index = index / 64;
    uint64_t bit_index = index % 64;
    bitmask->data[data_index] |= (1ULL << bit_index);
}
void heim_bitmask_unset(HeimBitmask* bitmask, uint64_t index) {
    if (index >= bitmask->size) {
        HEIM_LOG_ERROR("Bitmask index out of range");
        return;
    }
    uint64_t data_index = index / 64;
    uint64_t bit_index = index % 64;
    bitmask->data[data_index] &= ~(1ULL << bit_index);
}
bool heim_bitmask_get(HeimBitmask* bitmask, uint64_t index) {
    if (index >= bitmask->size) {
        HEIM_LOG_ERROR("Bitmask index out of range");
        return false;
    }
    uint64_t data_index = index / 64;
    uint64_t bit_index = index % 64;
    return (bitmask->data[data_index] & (1ULL << bit_index)) != 0;
}

void heim_bitmask_clear(HeimBitmask* bitmask) {
    uint64_t data_size = (bitmask->size / 64) + 1;
    heim_memzero(bitmask->data, data_size * 64);
}

void heim_bitmask_resize(HeimBitmask* bitmask, uint64_t size) {
    uint64_t data_size = (size / 64) + 1;
    if (data_size > (bitmask->size / 64) + 1) {
        uint64_t* new_data = HEIM_CALLOC(uint64_t, data_size, HEIM_MEMORY_TYPE_BASE);
        if (new_data == NULL) {
            HEIM_LOG_ERROR("Failed to allocate memory for bitmask");
            return;
        }
        heim_memcpy(new_data, bitmask->data, (bitmask->size / 64) + 1);
        HEIM_FREE(bitmask->data, HEIM_MEMORY_TYPE_BASE);
        bitmask->data = new_data;
    }
    bitmask->size = size;
}