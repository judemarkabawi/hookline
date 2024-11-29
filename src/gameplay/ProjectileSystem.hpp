#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "core/AssetManager.hpp"

class ProjectileSystem {
   public:
    void update(float dt, entt::registry &registry, entt::entity player);
    explicit ProjectileSystem() = default;
    explicit ProjectileSystem(AssetManager *asset_manager)
        : asset_manager_(asset_manager) {};

    void spawn_horizontal_projectile(float height, glm::vec2 position_range,
                                     float projectile_speed,
                                     float projectile_lifetime,
                                     entt::registry &registry);
    void spawn_vertical_projectile(float x_offset, glm::vec2 position_range,
                                   float projectile_speed,
                                   float projectile_lifetime,
                                   entt::registry &registry);
    void spawn_chasing_projectile(glm::vec2 origin, float projectile_speed,
                                  float projectile_lifetime,
                                  entt::registry &registry);
    void spawn_spray_projectile(glm::vec2 origin, int projectile_count,
                                float projectile_speed,
                                float projectile_lifetime,
                                entt::registry &registry);

   private:
    bool check_collision(const glm::vec2 &projectile_pos,
                         const glm::vec2 &player_pos);

    AssetManager *asset_manager_;
};