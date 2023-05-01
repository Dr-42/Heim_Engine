#ifndef HEIM_ECS_H
#define HEIM_ECS_H

#include <stdint.h>

#include "core/heim_memory.h"
#include "core/heim_logger.h"

#define MAX_COMPONENTS 64
#define MAX_ENTITIES 1024


typedef uint64_t HeimEntity;
typedef uint64_t HeimComponent;

typedef struct HeimEcs {
    HeimEntity *entities;
    HeimComponent *components;
    // Array of pointers system functions of type void funtion(HeimEcs *ecs, float dt)
    void (**systems)(struct HeimEcs*, float);

    void ***component_data; // [component_id][entity_id][component_data]

    uint64_t entity_count;
    uint64_t component_count;

    HeimMemory *memory;
    HeimLogger *logger;
} HeimEcs;

typedef void (*HeimSystem)(HeimEcs *ecs, float dt);

HeimEcs* heim_ecs_create(HeimLogger* logger, HeimMemory* memory);
void heim_ecs_free(HeimEcs* ecs, HeimMemory* memory);

HeimEntity heim_ecs_create_entity(HeimEcs* ecs);
void heim_ecs_destroy_entity(HeimEcs* ecs, HeimEntity entity);

HeimComponent heim_ecs_add_component(HeimEcs* ecs, HeimEntity entity, void* component, uint64_t size);
void heim_ecs_remove_component(HeimEcs* ecs, HeimEntity entity, HeimComponent component);

void heim_ecs_add_system(HeimEcs* ecs, HeimSystem system);
void heim_ecs_remove_system(HeimEcs* ecs, HeimSystem system);

void heim_ecs_update(HeimEcs* ecs, float dt);

#endif // HEIM_ECS_H
