#pragma once

struct HealthComponent {
    int health = 5;

    explicit HealthComponent(int initial_health = 5) : health(initial_health) {}

    void take_damage(int amount) { health = std::max(0, health - amount); }
};