#pragma once

#include <glm/glm.hpp>

struct HealthComponent {
    int health = 50;

    explicit HealthComponent(int initial_health = 50)
        : health(initial_health) {}

    void take_damage(int amount) { health = glm::max(0, health - amount); }
};