#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "constants.hpp"

struct GrapplingHookComponent {
    static constexpr float max_length = hookline::grapple_max_length;
    static constexpr float pull_force = hookline::grapple_pull_force;
    static constexpr float deactivate_distance =
        hookline::grapple_deactive_distance;

    entt::entity self;
    entt::entity user;
    bool attached = false;
    glm::vec2 attached_position;
    float curr_length = 0.0f;

    GrapplingHookComponent(entt::entity self_, entt::entity user_);

    void try_attach(glm::vec2 start_position, glm::vec2 target_position,
                    entt::registry &registry);
    void detach();

    glm::vec2 get_left_dir(glm::vec2 playerPos);
    glm::vec2 get_right_dir(glm::vec2 playerPos);
    glm::vec2 get_up_dir(glm::vec2 playerPos);
    glm::vec2 get_down_dir(glm::vec2 playerPos);
};