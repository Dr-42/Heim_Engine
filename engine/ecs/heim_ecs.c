#include "ecs/heim_ecs.h"

HeimEcs* heim_ecs_create(HeimMemory* memory, HeimLogger* logger){
    HeimEcs* ecs = heim_malloc(memory, sizeof(HeimEcs), HEIM_MEMORY_TYPE_ECS);
    ecs->memory = memory;
    ecs->logger = logger;
    ecs->entity_count = 0;
    ecs->component_count = 0;

    ecs->entities = HEIM_CALLOC(memory, HeimEntity, MAX_ENTITIES, HEIM_MEMORY_TYPE_ECS);
    ecs->components = HEIM_CALLOC(memory, uint64_t, MAX_COMPONENTS, HEIM_MEMORY_TYPE_ECS);
    ecs->systems = HEIM_CALLOC(memory, HeimSystem, MAX_COMPONENTS, HEIM_MEMORY_TYPE_ECS);

    ecs->component_data = HEIM_CALLOC(memory, void*, MAX_COMPONENTS, HEIM_MEMORY_TYPE_ECS);

    return ecs;
}

void heim_ecs_free(HeimEcs* ecs, HeimMemory* memory){
    HEIM_FREE(memory, ecs->entities, HEIM_MEMORY_TYPE_ECS);
    HEIM_FREE(memory, ecs->components, HEIM_MEMORY_TYPE_ECS);
    HEIM_FREE(memory, ecs->systems, HEIM_MEMORY_TYPE_ECS);
    HEIM_FREE(memory, ecs->component_data, HEIM_MEMORY_TYPE_ECS);
    HEIM_FREE(memory, ecs, HEIM_MEMORY_TYPE_ECS);
}

HeimEntity heim_ecs_create_entity(HeimEcs* ecs){
    HeimEntity entity = ecs->entity_count++;

    if (entity >= MAX_ENTITIES){
        HEIM_LOG_ERROR(ecs->logger, "Entity count exceeded MAX_ENTITIES");
        return 0;
    }

    for(uint64_t i = 0; i < MAX_ENTITIES; i++){
        if(ecs->entities[i] == 0){
            entity = i;
            return entity;
        }
    }
}

void heim_ecs_destroy_entity(HeimEcs* ecs, HeimEntity entity){
    ecs->entities[entity] = 0;
}

HeimComponent heim_ecs_add_component(HeimEcs* ecs, HeimEntity entity, void* component, uint64_t size){
    HeimComponent component_id = ecs->component_count++;
    if (component_id >= MAX_COMPONENTS){
        HEIM_LOG_ERROR(ecs->logger, "Component count exceeded MAX_COMPONENTS");
        return 0;
    }
    ecs->components[component_id] = component_id;

    if(ecs->component_data[component_id] == 0){
        ecs->component_data[component_id] = HEIM_CALLOC(ecs->memory, void*, MAX_ENTITIES, HEIM_MEMORY_TYPE_ECS);
    }

    ecs->component_data[component_id][entity] = component;
    ecs_matrix[entity][component_id] = true;
    return component_id;
}

void heim_ecs_remove_component(HeimEcs* ecs, HeimEntity entity, HeimComponent component){
    for(uint64_t i = 0; i < MAX_COMPONENTS; i++){
        if(ecs->components[i] == component){
            ecs->component_data[i][entity] = 0;
            ecs_matrix[entity][i] = false;
            return;
        }
    }

    HEIM_LOG_WARN(ecs->logger, "Component not found");
}

void heim_ecs_add_system(HeimEcs* ecs, HeimSystem system){
    for(uint64_t i = 0; i < MAX_COMPONENTS; i++){
        if(ecs->systems[i] == 0){
            ecs->systems[i] = system;
            return;
        }
    }
}

void heim_ecs_remove_system(HeimEcs* ecs, HeimSystem system){
    for(uint64_t i = 0; i < MAX_COMPONENTS; i++){
        if(ecs->systems[i] == system){
            ecs->systems[i] = 0;
            return;
        }
    }
}

void heim_ecs_update(HeimEcs* ecs, float dt){
    for(uint64_t i = 0; i < MAX_COMPONENTS; i++){
        if(ecs->systems[i] != 0){
            ecs->systems[i](ecs, dt);
        }
    }
}
