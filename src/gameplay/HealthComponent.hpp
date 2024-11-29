#pragma once

#include <glm/glm.hpp>

struct HealthComponent {
    int health = 50;
    int inital_health = 50;

    explicit HealthComponent(int initial_health_ = 50)
        : health(initial_health_), inital_health(initial_health_) {}

    void take_damage(int amount) { health = glm::max(0, health - amount); }

    void heal(int amount) { health = health + amount; }
};