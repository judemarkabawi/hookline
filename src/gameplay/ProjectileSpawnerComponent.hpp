#pragma once

#include <glm/glm.hpp>

enum class SpawnerType { Horizontal, Vertical, Chasing, Spray };


struct ProjectileSpawnerComponent {
    SpawnerType type;

    glm::vec2 position_range;  // For horizontal/vertical (min, max)
    float trigger_height = 0;  // For height-based spawners
    float interval = 0;        // Time between bursts
    float small_interval = 0;  // Time between individual shots in a burst
    int burst_count = 1;       // Number of shots per burst
    int projectile_count = 6;  // Spray: Number of projectiles in spray
    float projectile_speed = 0; // Speed of spawned projectiles
    float projectile_lifetime = 0; // Lifetime of spawned projectiles

    // Internal timers
    float timer = 0;           // General-purpose timer for interval
    int shots_remaining = 0;   // Remaining shots in the current burst

    explicit ProjectileSpawnerComponent(
        SpawnerType type_, glm::vec2 position_range_, float interval_,
        float small_interval_, int burst_count_, float projectile_speed_,
        float projectile_lifetime_, int projectile_count_ = 6,
        float trigger_height_ = 0)
        : type(type_),
          position_range(position_range_),
          trigger_height(trigger_height_),
          interval(interval_),
          small_interval(small_interval_),
          burst_count(burst_count_),
          projectile_count(projectile_count_),
          projectile_speed(projectile_speed_),
          projectile_lifetime(projectile_lifetime_) {}
};