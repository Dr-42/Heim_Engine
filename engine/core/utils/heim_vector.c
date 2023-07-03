#include "core/utils/heim_vector.h"

#include "core/heim_logger.h"
#include "core/heim_memory.h"

void* _heim_vector_create(uint64_t length, uint64_t stride) {
    uint64_t header_size = HEIM_VECTOR_FIELD_LENGTH * sizeof(uint64_t);
    uint64_t array_size = length * stride;
    uint64_t* new_array = heim_malloc(header_size + array_size, HEIM_MEMORY_TYPE_BASE, __FILE__, __LINE__);
    heim_memzero(new_array, header_size + array_size);
    new_array[HEIM_VECTOR_CAPACITY] = length;
    new_array[HEIM_VECTOR_LENGTH] = 0;
    new_array[HEIM_VECTOR_STRIDE] = stride;
    return (void*)(new_array + HEIM_VECTOR_FIELD_LENGTH);
}

void _heim_vector_destroy(void* array, const char* file, int line) {
    if (!array) {
        HEIM_LOG_WARN("Attempted to destroy null vector at %d in %s", line, file);
        return;
    }

    uint64_t* header = (uint64_t*)array - HEIM_VECTOR_FIELD_LENGTH;
    // uint64_t header_size = HEIM_VECTOR_FIELD_LENGTH * sizeof(uint64_t);
    //  uint64_t total_size = header_size + header[HEIM_VECTOR_CAPACITY] * header[HEIM_VECTOR_STRIDE];
    HEIM_FREE(header, HEIM_MEMORY_TYPE_BASE);
    array = NULL;
}

uint64_t _heim_vector_field_get(void* array, uint64_t field) {
    uint64_t* header = (uint64_t*)array - HEIM_VECTOR_FIELD_LENGTH;
    return header[field];
}

void _heim_vector_field_set(void* array, uint64_t field, uint64_t value) {
    uint64_t* header = (uint64_t*)array - HEIM_VECTOR_FIELD_LENGTH;
    header[field] = value;
}

void* _heim_vector_resize(void* array) {
    uint64_t length = heim_vector_length(array);
    uint64_t stride = heim_vector_stride(array);
    void* temp = _heim_vector_create(
        (HEIM_VECTOR_RESIZE_FACTOR * heim_vector_capacity(array)),
        stride);
    heim_memcpy(temp, array, length * stride);

    _heim_vector_field_set(temp, HEIM_VECTOR_LENGTH, length);
    _heim_vector_destroy(array, __FILE__, __LINE__);
    return temp;
}

void* _heim_vector_push(void* array, const void* value_ptr) {
    uint64_t length = heim_vector_length(array);
    uint64_t stride = heim_vector_stride(array);
    if (length >= heim_vector_capacity(array)) {
        array = _heim_vector_resize(array);
    }

    uint64_t addr = (uint64_t)array;
    addr += (length * stride);
    heim_memcpy((void*)addr, value_ptr, stride);
    _heim_vector_field_set(array, HEIM_VECTOR_LENGTH, length + 1);
    return array;
}

void _heim_vector_pop(void* array, void* dest) {
    uint64_t length = heim_vector_length(array);
    uint64_t stride = heim_vector_stride(array);
    uint64_t addr = (uint64_t)array;
    addr += ((length - 1) * stride);
    heim_memcpy(dest, (void*)addr, stride);
    _heim_vector_field_set(array, HEIM_VECTOR_LENGTH, length - 1);
}

void* _heim_vector_pop_at(void* array, uint64_t index, void* dest) {
    uint64_t length = heim_vector_length(array);
    uint64_t stride = heim_vector_stride(array);
    if (index >= length) {
        HEIM_LOG_ERROR("Index outside the bounds of this array! Length: %i, index: %index", length, index);
        return array;
    }

    uint64_t addr = (uint64_t)array;
    heim_memcpy(dest, (void*)(addr + (index * stride)), stride);

    // If not on the last element, snip out the entry and copy the rest inward.
    if (index != length - 1) {
        heim_memcpy(
            (void*)(addr + (index * stride)),
            (void*)(addr + ((index + 1) * stride)),
            stride * (length - index));
    }

    _heim_vector_field_set(array, HEIM_VECTOR_LENGTH, length - 1);
    return array;
}

void* _heim_vector_insert_at(void* array, uint64_t index, void* value_ptr) {
    uint64_t length = heim_vector_length(array);
    uint64_t stride = heim_vector_stride(array);
    if (index >= length) {
        HEIM_LOG_ERROR("Index outside the bounds of this array! Length: %i, index: %index", length, index);
        return array;
    }
    if (length >= heim_vector_capacity(array)) {
        array = _heim_vector_resize(array);
    }

    uint64_t addr = (uint64_t)array;

    // If not on the last element, copy the rest outward.
    if (index != length - 1) {
        heim_memcpy(
            (void*)(addr + ((index + 1) * stride)),
            (void*)(addr + (index * stride)),
            stride * (length - index));
    }

    // Set the value at the index
    heim_memcpy((void*)(addr + (index * stride)), value_ptr, stride);

    _heim_vector_field_set(array, HEIM_VECTOR_LENGTH, length + 1);
    return array;
}