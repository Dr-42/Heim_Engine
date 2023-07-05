#ifndef HEIM_BITMASK_H
#define HEIM_BITMASK_H

#include <stdbool.h>
#include <stdint.h>

typedef struct HeimBitmask {
    uint64_t* data;
    uint64_t size;
} HeimBitmask;

HeimBitmask* heim_bitmask_create(uint64_t size);
void heim_bitmask_destroy(HeimBitmask* bitmask);

void heim_bitmask_set(HeimBitmask* bitmask, uint64_t index);
void heim_bitmask_unset(HeimBitmask* bitmask, uint64_t index);
bool heim_bitmask_get(HeimBitmask* bitmask, uint64_t index);

void heim_bitmask_clear(HeimBitmask* bitmask);

void heim_bitmask_resize(HeimBitmask* bitmask, uint64_t size);

void heim_bitmask_push_set(HeimBitmask* bitmask);

#endif  // HEIM_BITMASK_H