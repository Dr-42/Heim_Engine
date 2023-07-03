#include "ecs/heim_ecs.h"

#include "ecs/heim_ecs_predef.h"
#include "ecs/predef_comps/heim_camera.h"
#include "ecs/predef_systems/heim_pbr_model_renderer.h"

static HeimEcs* ecs = NULL;

void heim_ecs_create() {
    ecs = HEIM_CALLOC(HeimEcs, 1, HEIM_MEMORY_TYPE_ECS);
    ecs->entity_count = 0;
    ecs->component_count = 0;

    ecs->entities = HEIM_CALLOC(HeimEntity, MAX_ENTITIES, HEIM_MEMORY_TYPE_ECS);
    ecs->components = HEIM_CALLOC(HeimComponent, MAX_COMPONENTS, HEIM_MEMORY_TYPE_ECS);
    ecs->systems = HEIM_CALLOC(HeimSystem, MAX_COMPONENTS, HEIM_MEMORY_TYPE_ECS);

    ecs->component_data = HEIM_CALLOC(void**, MAX_COMPONENTS, HEIM_MEMORY_TYPE_ECS);

    heim_ecs_load_predef_components();
    heim_load_predef_systems();
}

void heim_ecs_close() {
    for (uint64_t i = 0; i < ecs->component_count; i++) {
        if (ecs->component_data[i] != 0) {
            HEIM_FREE(ecs->component_data[i], HEIM_MEMORY_TYPE_ECS);
        }
    }
    void heim_unload_predef_systems();

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
    ecs->component_data[component_id] = heim_calloc(size, MAX_ENTITIES, HEIM_MEMORY_TYPE_ECS, __FILE__, __LINE__);

    return component_id;
}

void heim_ecs_add_component(HeimEntity entity, HeimComponent component, void* data) {
    ecs->component_data[component][entity] = data;
    ecs->component_masks[entity] |= (1 << component);
}

void heim_ecs_remove_component(HeimEntity entity, HeimComponent component) {
    ecs->component_data[component][entity] = NULL;
    ecs->component_masks[entity] &= ~(1 << component);  // Clear the bit for this component
}

bool heim_ecs_has_component(HeimEntity entity, HeimComponent component) {
    return ecs->component_masks[entity] & (1 << component);
}

void* heim_ecs_get_component_data(HeimEntity entity, HeimComponent component) {
    return ecs->component_data[component][entity];
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
            if (ecs->systems[i] == heim_pbr_model_renderer_system) {
                // Clear the camera framebuffers
                for (HeimEntity entity = 1; entity < ecs->entity_count + 1; entity++) {
                    if (heim_ecs_has_component(entity, get_camera_component())) {
                        HeimCamera* camera = heim_ecs_get_component_data(entity, get_camera_component());
                        heim_camera_clear(camera);
                    }
                }
            }
            for (HeimEntity entity = 1; entity < ecs->entity_count + 1; entity++) {
                ecs->systems[i](entity, dt);
            }
        }
    }
}