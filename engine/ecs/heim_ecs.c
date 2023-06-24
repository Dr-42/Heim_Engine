#include "ecs/heim_ecs.h"

static uint64_t ecs_matrix[MAX_ENTITIES][MAX_COMPONENTS] = {0};

static HeimEcs* ecs = NULL;

void heim_ecs_create() {
    ecs = HEIM_CALLOC(HeimEcs, 1, HEIM_MEMORY_TYPE_ECS);
    ecs->entity_count = 0;
    ecs->component_count = 0;

    ecs->entities = HEIM_CALLOC(HeimEntity, MAX_ENTITIES, HEIM_MEMORY_TYPE_ECS);
    ecs->components = HEIM_CALLOC(HeimComponent, MAX_COMPONENTS, HEIM_MEMORY_TYPE_ECS);
    ecs->systems = HEIM_CALLOC(HeimSystem, MAX_COMPONENTS, HEIM_MEMORY_TYPE_ECS);

    ecs->component_data = HEIM_CALLOC(void**, MAX_COMPONENTS, HEIM_MEMORY_TYPE_ECS);
}

void heim_ecs_close() {
    for (uint64_t i = 0; i < MAX_COMPONENTS; i++) {
        if (ecs->component_data[i] != 0) {
            HEIM_FREE(ecs->component_data[i], HEIM_MEMORY_TYPE_ECS);
        }
    }

    HEIM_FREE(ecs->component_data, HEIM_MEMORY_TYPE_ECS);
    HEIM_FREE(ecs->systems, HEIM_MEMORY_TYPE_ECS);
    HEIM_FREE(ecs->components, HEIM_MEMORY_TYPE_ECS);
    HEIM_FREE(ecs->entities, HEIM_MEMORY_TYPE_ECS);
    HEIM_FREE(ecs, HEIM_MEMORY_TYPE_ECS);
}

HeimEntity heim_ecs_create_entity() {
    HeimEntity entity = ecs->entity_count++;

    if (entity >= MAX_ENTITIES) {
        HEIM_LOG_ERROR("Entity count exceeded MAX_ENTITIES");
        return 0;
    }
    for (uint64_t i = 1; i < MAX_ENTITIES; i++) {
        if (ecs->entities[i] == 0) {
            entity = i;
            break;
        }
    }
    ecs->entities[entity] = entity;
    return entity;
}

void heim_ecs_destroy_entity(HeimEntity entity) {
    ecs->entities[entity] = 0;
}

HeimComponent heim_ecs_register_component(uint64_t size) {
    HeimComponent component_id = ecs->component_count++;
    if (component_id >= MAX_COMPONENTS) {
        HEIM_LOG_ERROR("Component count exceeded MAX_COMPONENTS");
        return 0;
    }
    for (uint64_t i = 1; i < MAX_COMPONENTS; i++) {
        if (ecs->components[i] == 0) {
            component_id = i;
            break;
        }
    }
    ecs->components[component_id] = component_id;
    if (ecs->component_data[component_id] != 0) {
        HEIM_FREE(ecs->component_data[component_id], HEIM_MEMORY_TYPE_ECS);
    }
    ecs->component_data[component_id] = heim_calloc(size, MAX_ENTITIES, HEIM_MEMORY_TYPE_ECS);

    return component_id;
}

void heim_ecs_add_component(HeimEntity entity, HeimComponent component, void* data) {
    for (uint64_t i = 0; i < MAX_COMPONENTS; i++) {
        if (ecs->components[i] == component) {
            ecs->component_data[i][entity] = data;
            ecs_matrix[entity][i] = true;
            return;
        }
    }

    HEIM_LOG_WARN("Component not found");
}

void heim_ecs_remove_component(HeimEntity entity, HeimComponent component) {
    for (uint64_t i = 0; i < MAX_COMPONENTS; i++) {
        if (ecs->components[i] == component) {
            ecs->component_data[i][entity] = 0;
            ecs_matrix[entity][i] = false;
            return;
        }
    }

    HEIM_LOG_WARN("Component not found");
}

void heim_ecs_add_system(HeimSystem system) {
    for (uint64_t i = 0; i < MAX_COMPONENTS; i++) {
        if (ecs->systems[i] == 0) {
            ecs->systems[i] = system;
            return;
        }
    }
}

void heim_ecs_remove_system(HeimSystem system) {
    for (uint64_t i = 0; i < MAX_COMPONENTS; i++) {
        if (ecs->systems[i] == system) {
            ecs->systems[i] = 0;
            return;
        }
    }
}

void heim_ecs_update(float dt) {
    for (uint64_t i = 0; i < MAX_COMPONENTS; i++) {
        if (ecs->systems[i] != 0) {
            ecs->systems[i](ecs, dt);
        }
    }
}
