#pragma once

#include <glm/glm.hpp>

enum class ProjectileType { Horizontal, Chasing, Spray };

struct ProjectileComponent {
    ProjectileType type;
    glm::vec2 direction;
    float speed;
    float lifetime;
    float currtime = 0;

    explicit ProjectileComponent(ProjectileType type_, glm::vec2 direction_,
                                 float speed_, float lifetime_)
        : type(type_),
          direction(direction_),
          speed(speed_),
          lifetime(lifetime_) {}
};