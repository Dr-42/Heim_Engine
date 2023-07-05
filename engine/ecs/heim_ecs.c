#include "ecs/heim_ecs.h"

#include "core/utils/heim_vector.h"
#include "ecs/heim_ecs_predef.h"
#include "ecs/predef_comps/heim_camera.h"
#include "ecs/predef_systems/heim_pbr_model_renderer.h"

static HeimEcs* ecs = NULL;

void heim_ecs_create() {
    ecs = HEIM_CALLOC(HeimEcs, 1, HEIM_MEMORY_TYPE_ECS);
    ecs->entities = heim_bitmask_create(MAX_ENTITIES);
    ecs->components = heim_bitmask_create(0);
    ecs->component_masks = heim_vector_create(HeimBitmask*);
    ecs->entity_count = 0;
    ecs->component_count = 0;
    ecs->component_data = heim_vector_create(HeimComponentData*);

    ecs->systems = heim_vector_create(HeimSystem);

    heim_ecs_load_predef_components();
    heim_load_predef_systems();
}

void heim_ecs_close() {
    for (uint64_t i = 0; i < ecs->component_count; i++) {
        if (ecs->component_data[i] != 0) {
            HEIM_FREE(ecs->component_data[i], HEIM_MEMORY_TYPE_ECS);
        }
    }
    heim_unload_predef_systems();

    for (size_t i = 0; i < heim_vector_length(ecs->component_masks); i++) {
        heim_bitmask_destroy(ecs->component_masks[i]);
    }
    heim_vector_destroy(ecs->component_masks);
    heim_bitmask_destroy(ecs->entities);
    heim_bitmask_destroy(ecs->components);
    heim_vector_destroy(ecs->systems);
    heim_vector_destroy(ecs->component_data);
    HEIM_FREE(ecs, HEIM_MEMORY_TYPE_ECS);
}

HeimEntity heim_ecs_create_entity() {
    HeimEntity entity = ecs->entity_count++;

    if (entity >= MAX_ENTITIES) {
        HEIM_LOG_ERROR("Entity count exceeded MAX_ENTITIES");
        return 0;
    }
    for (uint64_t i = 1; i < MAX_ENTITIES; i++) {
        if (heim_bitmask_get(ecs->entities, i) == false) {
            entity = i;
            break;
        }
    }
    heim_bitmask_set(ecs->entities, entity);
    HeimBitmask* component_mask = heim_bitmask_create(ecs->components->size);
    if (entity == ecs->entity_count) {
        heim_vector_push(ecs->component_masks, component_mask);
    } else {
        ecs->component_masks[entity - 1] = component_mask;
    }

    return entity;
}

void heim_ecs_destroy_entity(HeimEntity entity) {
    heim_bitmask_unset(ecs->entities, entity);
}

HeimComponent heim_ecs_register_component() {
    HeimComponent component_id = ecs->components->size;
    ecs->component_count++;
    heim_bitmask_push_set(ecs->components);
    HeimComponentData* comps_arr = HEIM_CALLOC(HeimComponentData, MAX_ENTITIES, HEIM_MEMORY_TYPE_ECS);
    heim_vector_push(ecs->component_data, comps_arr);
    return component_id;
}

void heim_ecs_add_component(HeimEntity entity, HeimComponent component, void* data) {
    HeimComponentData* component_data = ecs->component_data[component];
    component_data[entity].entity = entity;
    component_data[entity].data = data;
    heim_bitmask_set(ecs->component_masks[entity - 1], component);
}

void heim_ecs_remove_component(HeimEntity entity, HeimComponent component) {
    HeimComponentData* component_data = ecs->component_data[component];
    component_data[entity].entity = 0;
    component_data[entity].data = NULL;
    heim_bitmask_unset(ecs->component_masks[entity - 1], component);
}

bool heim_ecs_has_component(HeimEntity entity, HeimComponent component) {
    return heim_bitmask_get(ecs->component_masks[entity - 1], component);
}

void* heim_ecs_get_component_data(HeimEntity entity, HeimComponent component) {
    HeimComponentData* component_data = ecs->component_data[component];
    return component_data[entity].data;
}

void heim_ecs_register_system(HeimSystem system) {
    heim_vector_push(ecs->systems, system);
}

void heim_ecs_update(float dt) {
    for (uint64_t i = 0; i < heim_vector_length(ecs->systems); i++) {
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