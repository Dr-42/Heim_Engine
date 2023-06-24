#include <core/heim_engine.h>

HeimEngine *heim;

HeimEntity entity, entity2;
HeimComponent component;

typedef struct {
    HeimVec2f position;
} Position;

Position position = {(HeimVec2f){0.0f, 0.0f}};
Position position2 = {(HeimVec2f){10.0f, 0.0f}};

float total_time = 0.0f;

void test_system(HeimEcs *ecs, float dt) {
    for (uint64_t i = 1; i < ecs->entity_count + 1; i++) {
        HeimEntity entity = ecs->entities[i];
        for (uint64_t j = 1; j < ecs->component_count + 1; j++) {
            if (ecs->components[j] == component) {
                Position *pos = ecs->component_data[j][entity];
                pos->position = heim_vec2f_add(pos->position, (HeimVec2f){dt, 0.1f});
                // HEIM_LOG_DEBUG("Position: %f, %f", pos->position.x, pos->position.y);
            }
        }
    }
}

void testbed_init() {
    HEIM_LOG_INFO("Testbed init");
    entity = heim_ecs_create_entity();
    entity2 = heim_ecs_create_entity();

    component = heim_ecs_register_component(sizeof(Position));

    heim_ecs_add_component(entity, component, &position);
    heim_ecs_add_component(entity2, component, &position2);

    heim_ecs_add_system(test_system);
}

void testbed_update(float *dt) {
    (void)dt;
    if (heim_input_key_pressed(GLFW_KEY_ESCAPE)) {
        heim_engine_should_close(true);
    }
}

int main(void) {
    heim_engine_new("Heim Engine Testbed");
    heim_engine_set_window_size(800, 600);
    heim_engine_set_window_top_left(100, 100);

    heim_engine_init(testbed_init);
    heim_engine_run(testbed_update);
    heim_engine_shutdown();
    return 0;
}