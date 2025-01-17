/**
 * TransformComponent contains the position, scale, and rotation of an object.
 */
#pragma once

#include <glm/glm.hpp>

struct TransformComponent {
    static constexpr glm::vec2 default_scale = {0.05f, 0.05f};
    static constexpr float default_rotation = 0.0f;

    glm::vec2 position = {0.0f, 0.0f};
    glm::vec2 scale = default_scale;
    float rotation = default_rotation;

    explicit TransformComponent() = default;
    explicit TransformComponent(glm::vec2 position_,
                                glm::vec2 scale_ = default_scale,
                                float rotation_ = default_rotation)
        : position(position_), scale(scale_), rotation(rotation_) {}
};
