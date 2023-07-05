#ifndef HEIM_ECS_H
#define HEIM_ECS_H

#include <stdbool.h>
#include <stdint.h>

#include "core/heim_logger.h"
#include "core/heim_memory.h"
#include "core/utils/heim_bitmask.h"

#define MAX_COMPONENTS 64
#define MAX_ENTITIES 1024

typedef struct HeimEcs HeimEcs;

/// @brief A unique identifier for an entity
typedef uint64_t HeimEntity;

/// @brief A unique identifier for a component
typedef uint64_t HeimComponent;

/// @brief A function pointer to a system function of type void funtion(HeimEcs *ecs, float dt)
typedef void (*HeimSystem)(HeimEntity entity, float dt);

/// @brief A struct containing the entity and the component data
typedef struct HeimComponentData {
    HeimEntity entity;
    void* data;
} HeimComponentData;

/// @brief A struct containing all the data needed for the ECS
typedef struct HeimEcs {
    HeimBitmask* entities;
    HeimBitmask* components;
    HeimSystem* systems;

    HeimComponentData* component_data[MAX_COMPONENTS];
    HeimBitmask** component_masks;

    uint64_t entity_count;
    uint64_t component_count;

} HeimEcs;

/// @brief Creates a new ECS Manager
/// @return Pointer to the new HeimEcs
void heim_ecs_create();

/// @brief Frees the memory allocated by the ECS
void heim_ecs_close();

/// @brief Creates a new entity
/// @return New entity id
HeimEntity heim_ecs_create_entity();

/// @brief Destroys an entity
/// @param entity Entity id to destroy
void heim_ecs_destroy_entity(HeimEntity entity);

/// @brief Registers a new component type
/// @return The component id
HeimComponent heim_ecs_register_component();

/// @brief Adds a component to an entity
/// @param entity The entity to add the component to
/// @param component The component type to add
/// @param data The pointer to component data
void heim_ecs_add_component(HeimEntity entity, HeimComponent component, void* data);

/// @brief Removes a component from an entity
/// @param entity The entity to remove the component from
/// @param component The component type to remove
void heim_ecs_remove_component(HeimEntity entity, HeimComponent component);

/// @brief Checks if an entity has a component
/// @param entity The entity to check
/// @param component The component type to check
bool heim_ecs_has_component(HeimEntity entity, HeimComponent component);

/// @brief Gets the component data of an entity
/// @param entity Entity id
/// @param component Component id
/// @return Component data pointer
void* heim_ecs_get_component_data(HeimEntity entity, HeimComponent component);

/// @brief Registers a new system
/// @param system Function pointer to the system function
void heim_ecs_register_system(HeimSystem system);

/// @brief Internal function to update the ECS
/// @param dt Delta time
void heim_ecs_update(float dt);
#endif  // HEIM_ECS_H
