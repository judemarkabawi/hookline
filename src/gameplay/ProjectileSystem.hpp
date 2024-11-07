#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include "ProjectileComponent.hpp"
#include "core/TransformComponent.hpp"
#include "core/RenderComponent.hpp"
#include "gameplay/HealthComponent.hpp"

class ProjectileSystem {
public:
    void update(float dt, entt::registry &registry, entt::entity player);
    void spawn_horizontal_projectile(bool from_left, float height, entt::registry &registry);
    void spawn_chasing_projectile(glm::vec2 origin, entt::registry &registry);
    void spawn_spray_projectile(glm::vec2 origin, entt::registry &registry);

private:
    bool check_collision(const glm::vec2 &projectile_pos, const glm::vec2 &player_pos);
};
