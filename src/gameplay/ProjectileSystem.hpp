#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "core/AssetManager.hpp"

class ProjectileSystem {
   public:
    void update(float dt, entt::registry &registry, entt::entity player);
    void spawn_horizontal_projectile(bool from_left, float height,
                                     entt::registry &registry);
    void spawn_chasing_projectile(glm::vec2 origin, entt::registry &registry);
    void spawn_spray_projectile(glm::vec2 origin, entt::registry &registry);

    explicit ProjectileSystem() = default;
    explicit ProjectileSystem(AssetManager *asset_manager) : asset_manager_(asset_manager) {};

   private:
    bool check_collision(const glm::vec2 &projectile_pos,
                         const glm::vec2 &player_pos);
    float timer_h = 0.0f;
    float timer_c = 0.0f;
    float timer_s = 0.0f;

    AssetManager *asset_manager_;
};