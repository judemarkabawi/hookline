#pragma once

#include <glm/glm.hpp>

enum class ProjectileType { Horizontal, Vertical, Chasing, Spray };

struct ProjectileComponent {
    ProjectileType type;
    glm::vec2 direction;
    float speed;
    float lifetime;
    glm::vec2 range;
    float height;

    float currtime = 0;

    explicit ProjectileComponent(ProjectileType type_, glm::vec2 direction_,
                                 float speed_, float lifetime_,
                                 glm::vec2 range_, float height_)
        : type(type_),
          direction(direction_),
          speed(speed_),
          lifetime(lifetime_),
          range(range_),
          height(height_) {}
};