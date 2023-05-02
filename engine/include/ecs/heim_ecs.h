#ifndef HEIM_ECS_H
#define HEIM_ECS_H

#include <stdint.h>

#include "core/heim_memory.h"
#include "core/heim_logger.h"

#define MAX_COMPONENTS 64
#define MAX_ENTITIES 1024


/// @brief A unique identifier for an entity
typedef uint64_t HeimEntity;

/// @brief A unique identifier for a component
typedef uint64_t HeimComponent;

/// @brief A struct containing all the data needed for the ECS
typedef struct HeimEcs {
    HeimEntity *entities;
    HeimComponent *components;
    void (**systems)(struct HeimEcs*, float);   // Array of pointers system functions of type void funtion(HeimEcs *ecs, float dt)

    void ***component_data;                     // [component_id][entity_id][component_data]

    uint64_t entity_count;
    uint64_t component_count;

    HeimLogger *logger;
    HeimMemory *memory;
} HeimEcs;

/// @brief A function pointer to a system function of type void funtion(HeimEcs *ecs, float dt)
typedef void (*HeimSystem)(HeimEcs *ecs, float dt);

/// @brief Creates a new ECS Manager
/// @param logger Pointer to the HeimLogger
/// @param memory Pointer to the HeimMemory
/// @return Pointer to the new HeimEcs
HeimEcs* heim_ecs_create(HeimLogger* logger, HeimMemory* memory);

/// @brief Frees the memory allocated by the ECS
/// @param ecs Pointer to the HeimEcs
/// @param memory Pointer to the HeimMemory
void heim_ecs_free(HeimEcs* ecs, HeimMemory* memory);

/// @brief Creates a new entity
/// @param ecs The HeimEcs
/// @return New entity id
HeimEntity heim_ecs_create_entity(HeimEcs* ecs);

/// @brief Destroys an entity
/// @param ecs The HeimEcs
/// @param entity Entity id to destroy
void heim_ecs_destroy_entity(HeimEcs* ecs, HeimEntity entity);

/// @brief Registers a new component type
/// @param ecs The HeimEcs
/// @param size Size of the component data. Pass as sizeof(YourComponent)
/// @return The component id
HeimComponent heim_ecs_register_component(HeimEcs *ecs, uint64_t size);

/// @brief Adds a component to an entity
/// @param ecs The HeimEcs
/// @param entity The entity to add the component to
/// @param component The component type to add
/// @param data The pointer to component data
void heim_ecs_add_component(HeimEcs* ecs, HeimEntity entity, HeimComponent component, void* data);

/// @brief Removes a component from an entity
/// @param ecs The HeimEcs
/// @param entity The entity to remove the component from
/// @param component The component type to remove
void heim_ecs_remove_component(HeimEcs* ecs, HeimEntity entity, HeimComponent component);

/// @brief Registers a new system
/// @param ecs The HeimEcs
/// @param system Function pointer to the system function
void heim_ecs_add_system(HeimEcs* ecs, HeimSystem system);

/// @brief Removes a system
/// @param ecs The HeimEcs
/// @param system Function pointer to the system function
void heim_ecs_remove_system(HeimEcs* ecs, HeimSystem system);

/// @brief Internal function to update the ECS
/// @param ecs The HeimEcs
/// @param dt Delta time
void heim_ecs_update(HeimEcs* ecs, float dt);

#endif // HEIM_ECS_H
