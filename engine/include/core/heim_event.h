#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "math/heim_vec.h"

typedef union HeimEventData {
    float f[4];
    int32_t i[4];
    uint32_t u[4];
    void *p[4];
    uint64_t u64[2];
    int64_t i64[2];
    double d[2];
    HeimVec4f vec4;
    HeimVec2f vec2[2];
} HeimEventData;

typedef enum HeimEventType {
    HEIM_EVENT_TYPE_NONE,
    HEIM_EVENT_TYPE_KEY_DOWN,
    HEIM_EVENT_TYPE_KEY_UP,
    HEIM_EVENT_TYPE_MOUSE_DOWN,
    HEIM_EVENT_TYPE_MOUSE_UP,
    HEIM_EVENT_TYPE_MOUSE_MOVE,
    HEIM_EVENT_TYPE_MOUSE_SCROLL,
    HEIM_EVENT_TYPE_WINDOW_RESIZE,
    HEIM_EVENT_TYPE_WINDOW_CLOSE,
    HEIM_EVENT_TYPE_WINDOW_FOCUS,
    HEIM_EVENT_TYPE_WINDOW_LOST_FOCUS,
    HEIM_EVENT_TYPE_WINDOW_MOVED,
    HEIM_EVENT_TYPE_COUNT
} HeimEventType;

typedef struct HeimEvent {
    HeimEventType type;
    HeimEventData data;
} HeimEvent;

typedef struct HeimEventQueue {
    HeimEvent *events;
    uint32_t count;
} HeimEventQueue;

HeimEventQueue *heim_event_queue_create(uint32_t size);
void heim_event_queue_destroy(HeimEventQueue *queue);

void heim_event_push(HeimEventQueue *queue, HeimEvent event);
HeimEvent heim_event_pop(HeimEventQueue *queue);
HeimEvent heim_event_peek(HeimEventQueue *queue);

void heim_event_queue_clear(HeimEventQueue *queue);
bool heim_event_queue_is_empty(HeimEventQueue *queue);
