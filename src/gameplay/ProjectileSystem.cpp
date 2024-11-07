#include "ProjectileSystem.hpp"

#include "util/misc.hpp"

void ProjectileSystem::update(float dt, entt::registry &registry, entt::entity player) {
    auto &player_transform = registry.get<TransformComponent>(player);
    auto &player_health = registry.get<HealthComponent>(player);

    registry.view<TransformComponent, ProjectileComponent>().each(
        [&](auto entity, auto &transform, auto &projectile) {
            switch (projectile.type) {
                case ProjectileType::Horizontal:
                    transform.position += projectile.direction * projectile.speed * dt;
                    if (transform.position.x > 1.0f || transform.position.x < -1.0f) {
                        projectile.direction = -projectile.direction;
                    }
                    break;

                case ProjectileType::Chasing:
                    projectile.direction = glm::normalize(player_transform.position - transform.position);
                    transform.position += projectile.direction * projectile.speed * dt;
                    break;

                case ProjectileType::Spray:
                    transform.position += projectile.direction * projectile.speed * dt;
                    break;
            }

            // Collision check with player
            if (check_collision(transform.position, player_transform.position)) {
                registry.destroy(entity);  // Destroy projectile on hit
                player_health.take_damage(1);
            }
            projectile.currtime += dt;
            if (projectile.currtime >= projectile.lifetime) {
                registry.destroy(entity);
            }
        });
}

void ProjectileSystem::spawn_horizontal_projectile(bool from_left, float height, entt::registry &registry) {
    auto projectile = registry.create();
    glm::vec2 position = from_left ? glm::vec2{-1.0f, height} : glm::vec2{1.0f, height};
    glm::vec2 direction = from_left ? glm::vec2{1.0f, 0.0f} : glm::vec2{-1.0f, 0.0f};
    registry.emplace<TransformComponent>(projectile, position);
    registry.emplace<ProjectileComponent>(projectile, ProjectileType::Horizontal, direction, 0.5f, 100.0f);
    registry.emplace<RenderComponent>(projectile,
                                      hookline::get_basic_shape_debug());
}

void ProjectileSystem::spawn_chasing_projectile(glm::vec2 origin, entt::registry &registry) {
    auto projectile = registry.create();
    registry.emplace<TransformComponent>(projectile, origin);
    registry.emplace<ProjectileComponent>(projectile, ProjectileType::Chasing, glm::vec2{0.0f, 1.0f}, 0.4f, 10.0f);
    registry.emplace<RenderComponent>(projectile,
                                      hookline::get_basic_shape_debug());
}

void ProjectileSystem::spawn_spray_projectile(glm::vec2 origin, entt::registry &registry) {
    for (int i = 0; i < 6; ++i) {
        float angle = glm::radians(i * 60.0f);  // 60-degree increments for six directions
        glm::vec2 direction = glm::vec2(cos(angle), sin(angle));
        auto projectile = registry.create();
        registry.emplace<TransformComponent>(projectile, origin);
        registry.emplace<ProjectileComponent>(projectile, ProjectileType::Spray, direction, 0.6f, 10.0f);
        registry.emplace<RenderComponent>(projectile,
                                      hookline::get_basic_shape_debug());
    }
}

bool ProjectileSystem::check_collision(const glm::vec2 &projectile_pos, const glm::vec2 &player_pos) {
    return glm::distance(projectile_pos, player_pos) < 0.05f; 
}
