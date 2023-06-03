#include "ecs/heim_ecs.h"

static uint64_t ecs_matrix[MAX_ENTITIES][MAX_COMPONENTS] = {0};

HeimEcs* heim_ecs_create(HeimLogger* logger, HeimMemory* memory){
    HeimEcs *ecs = HEIM_CALLOC(memory, HeimEcs, 1, HEIM_MEMORY_TYPE_ECS);
    ecs->memory = memory;
    ecs->logger = logger;
    ecs->entity_count = 0;
    ecs->component_count = 0;

    ecs->entities = HEIM_CALLOC(memory, HeimEntity, MAX_ENTITIES, HEIM_MEMORY_TYPE_ECS);
    ecs->components = HEIM_CALLOC(memory, HeimComponent, MAX_COMPONENTS, HEIM_MEMORY_TYPE_ECS);
    ecs->systems = HEIM_CALLOC(memory, HeimSystem, MAX_COMPONENTS, HEIM_MEMORY_TYPE_ECS);

    ecs->component_data = HEIM_CALLOC(memory, void**, MAX_COMPONENTS, HEIM_MEMORY_TYPE_ECS);

    return ecs;
}

void heim_ecs_free(HeimEcs* ecs, HeimMemory* memory){
    for (uint64_t i = 0; i < MAX_COMPONENTS; i++){
        if (ecs->component_data[i] != 0){
            HEIM_FREE(memory, ecs->component_data[i], HEIM_MEMORY_TYPE_ECS);
        }
    }

    HEIM_FREE(memory, ecs->component_data, HEIM_MEMORY_TYPE_ECS);
    HEIM_FREE(memory, ecs->systems, HEIM_MEMORY_TYPE_ECS);
    HEIM_FREE(memory, ecs->components, HEIM_MEMORY_TYPE_ECS);
    HEIM_FREE(memory, ecs->entities, HEIM_MEMORY_TYPE_ECS);
    HEIM_FREE(memory, ecs, HEIM_MEMORY_TYPE_ECS);
}

HeimEntity heim_ecs_create_entity(HeimEcs* ecs){
    HeimEntity entity = ecs->entity_count++;

    if (entity >= MAX_ENTITIES){
        HEIM_LOG_ERROR(ecs->logger, "Entity count exceeded MAX_ENTITIES");
        return 0;
    }
    for (uint64_t i = 1; i < MAX_ENTITIES; i++){
        if (ecs->entities[i] == 0){
            entity = i;
            break;
        }
    }
    ecs->entities[entity] = entity;
    return entity;
}

void heim_ecs_destroy_entity(HeimEcs* ecs, HeimEntity entity){
    ecs->entities[entity] = 0;
}

HeimComponent heim_ecs_register_component(HeimEcs *ecs, uint64_t size){
    HeimComponent component_id = ecs->component_count++;
    if (component_id >= MAX_COMPONENTS){
        HEIM_LOG_ERROR(ecs->logger, "Component count exceeded MAX_COMPONENTS");
        return 0;
    }
    for (uint64_t i = 1; i < MAX_COMPONENTS; i++){
        if (ecs->components[i] == 0){
            component_id = i;
            break;
        }
    }

    ecs->components[component_id] = component_id;

    if(ecs->component_data[component_id] != 0){
        HEIM_FREE(ecs->memory, ecs->component_data[component_id], HEIM_MEMORY_TYPE_ECS);
    }
    ecs->component_data[component_id] = HEIM_CALLOC_SIZED(ecs->memory, void*, MAX_ENTITIES, size, HEIM_MEMORY_TYPE_ECS);

    return component_id;
}

void heim_ecs_add_component(HeimEcs* ecs, HeimEntity entity, HeimComponent component, void* data){
    for(uint64_t i = 0; i < MAX_COMPONENTS; i++){
        if(ecs->components[i] == component){
            ecs->component_data[i][entity] = data;
            ecs_matrix[entity][i] = true;
            return;
        }
    }

    HEIM_LOG_WARN(ecs->logger, "Component not found");
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
