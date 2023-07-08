#include "core/heim_event.h"

#include "core/heim_memory.h"
#include "core/utils/heim_vector.h"

HeimEventQueue *heim_event_queue_create() {
    HeimEventQueue *queue = HEIM_MALLOC(HeimEventQueue, HEIM_MEMORY_TYPE_BASE);
    queue->events = heim_vector_create(HeimEvent);
    queue->count = 0;
    return queue;
}
void heim_event_queue_destroy(HeimEventQueue *queue) {
    heim_vector_destroy(queue->events);
    HEIM_FREE(queue, HEIM_MEMORY_TYPE_BASE);
}

void heim_event_push(HeimEventQueue *queue, HeimEvent event) {
    heim_vector_push(queue->events, event);
    queue->count++;
}
HeimEvent heim_event_pop(HeimEventQueue *queue) {
    HeimEvent event;
    heim_vector_pop(queue->events, &event);
    queue->count--;
    return event;
}

HeimEvent heim_event_peek(HeimEventQueue *queue) {
    HeimEvent event = queue->events[queue->count - 1];
    return event;
}

void heim_event_queue_clear(HeimEventQueue *queue) {
    heim_vector_clear(queue->events);
    queue->count = 0;
}

bool heim_event_queue_is_empty(HeimEventQueue *queue) {
    return queue->count == 0;
}
