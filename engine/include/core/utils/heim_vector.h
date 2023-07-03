#ifndef HEIM_VECTOR_H
#define HEIM_VECTOR_H

#include <stdint.h>

/*
Memory layout
uint64_t capacity = number elements that can be held
uint64_t length = number of elements currently contained
uint64_t stride = size of each element in bytes
void* elements
*/

enum {
    HEIM_VECTOR_CAPACITY,
    HEIM_VECTOR_LENGTH,
    HEIM_VECTOR_STRIDE,
    HEIM_VECTOR_FIELD_LENGTH
};

void* _heim_vector_create(uint64_t length, uint64_t stride);
void _heim_vector_destroy(void* array, const char* file, int line);

uint64_t _heim_vector_field_get(void* array, uint64_t field);
void _heim_vector_field_set(void* array, uint64_t field, uint64_t value);

void* _heim_vector_resize(void* array);

void* _heim_vector_push(void* array, const void* value_ptr);
void _heim_vector_pop(void* array, void* dest);

void* _heim_vector_pop_at(void* array, uint64_t index, void* dest);
void* _heim_vector_insert_at(void* array, uint64_t index, void* value_ptr);

#define HEIM_VECTOR_DEFAULT_CAPACITY 1
#define HEIM_VECTOR_RESIZE_FACTOR 2

#define heim_vector_create(type) \
    _heim_vector_create(HEIM_VECTOR_DEFAULT_CAPACITY, sizeof(type))

#define heim_vector_reserve(type, capacity) \
    _heim_vector_create(capacity, sizeof(type))

#define heim_vector_destroy(array) _heim_vector_destroy(array, __FILE__, __LINE__);

#define heim_vector_push(array, value)           \
    {                                            \
        typeof(value) temp = value;              \
        array = _heim_vector_push(array, &temp); \
    }

#define heim_vector_pop(array, value_ptr) \
    _heim_vector_pop(array, value_ptr)

#define heim_vector_insert_at(array, index, value)           \
    {                                                        \
        typeof(value) temp = value;                          \
        array = _heim_vector_insert_at(array, index, &temp); \
    }

#define heim_vector_pop_at(array, index, value_ptr) \
    _heim_vector_pop_at(array, index, value_ptr)

#define heim_vector_clear(array) \
    _heim_vector_field_set(array, HEIM_VECTOR_LENGTH, 0)

#define heim_vector_capacity(array) \
    _heim_vector_field_get(array, HEIM_VECTOR_CAPACITY)

#define heim_vector_length(array) \
    _heim_vector_field_get(array, HEIM_VECTOR_LENGTH)

#define heim_vector_stride(array) \
    _heim_vector_field_get(array, HEIM_VECTOR_STRIDE)

#define heim_vector_length_set(array, value) \
    _heim_vector_field_set(array, HEIM_VECTOR_LENGTH, value)

#endif  // HEIM_VECTOR_H