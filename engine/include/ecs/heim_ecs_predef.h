#ifndef HEIM_ECS_PREDEF_H
#define HEIM_ECS_PREDEF_H

#include <stdint.h>

void heim_ecs_load_predef_components();
void heim_load_predef_systems();
void heim_unload_predef_systems();

uint64_t get_ui_transform_component();
uint64_t get_transform_component();
uint64_t get_ui_sprite_component();
uint64_t get_model_component();
uint64_t get_pbr_model_component();
uint64_t get_camera_component();

#endif  // HEIM_ECS_PREDEF_H