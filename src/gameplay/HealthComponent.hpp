#pragma once

#include <glm/glm.hpp>

struct HealthComponent {
    int health =10;

    explicit HealthComponent(int initial_health = 10)
        : health(initial_health) {}

    void take_damage(int amount) { health = glm::max(0, health - amount); }

    void heal(int amount) {health = health + amount;}
};