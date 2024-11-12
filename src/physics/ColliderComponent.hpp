#pragma once

#include <glm/glm.hpp>

/**
 * Component in any objects than can collide (2D Axis Aligned Bounding Box)
 *
 * Width/height should be half of the object's relative width/height. We assume
 * the position is at the center of the object
 */
struct ColliderComponent {
    glm::vec2 size;
    bool can_collide = true;
    bool can_move = true;
    bool hookable = true;

    ColliderComponent(glm::vec2 size_);

    // Builders
    ColliderComponent &set_size(glm::vec2 size);
    ColliderComponent &set_can_collide(bool can_collide);
    ColliderComponent &set_can_move(bool can_move);
    ColliderComponent &set_hookable(bool hookable);
};