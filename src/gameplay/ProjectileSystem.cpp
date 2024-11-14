#include "ProjectileSystem.hpp"

#include <cmath>
#include <random>

#include "core/TransformComponent.hpp"
#include "gameplay/HealthComponent.hpp"
#include "gameplay/ProjectileComponent.hpp"
#include "render/RenderComponent.hpp"
#include "util/misc.hpp"

float getRandomFloat(float min = -3.0f, float max = 3.0f) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

void ProjectileSystem::update(float dt, entt::registry &registry,
                              entt::entity player) {
    auto &player_transform = registry.get<TransformComponent>(player);
    auto &player_health = registry.get<HealthComponent>(player);

    timer_h += dt;
    if (timer_h >= 4) {
        timer_h = 0;
        spawn_horizontal_projectile(true, getRandomFloat(), registry);
    }
    timer_c += dt;
    if (timer_c >= 5) {
        timer_c = 0;
        spawn_chasing_projectile(glm::vec2(getRandomFloat(-1.0f, 1.0f), 1.0f),
                                 registry);
    }
    timer_s += dt;
    if (timer_s >= 9) {
        timer_s = 0;
        float height_ = getRandomFloat();
        if (std::abs(height_ - player_transform.position.y) < 0.2f) {
            height_ = height_ - 0.2f;
        }
        spawn_spray_projectile(glm::vec2(getRandomFloat(-3.0, 3.0f), height_),
                               registry);
    }

    registry.view<TransformComponent, ProjectileComponent>().each(
        [&](auto entity, auto &transform, auto &projectile) {
            switch (projectile.type) {
                case ProjectileType::Horizontal:
                    transform.position +=
                        projectile.direction * projectile.speed * dt;
                    if (transform.position.x > 3.0f ||
                        transform.position.x < -3.0f) {
                        projectile.direction = -projectile.direction;
                    }
                    break;

                case ProjectileType::Chasing:
                    projectile.direction = glm::normalize(
                        player_transform.position - transform.position);
                    transform.position +=
                        projectile.direction * projectile.speed * dt;
                    break;

                case ProjectileType::Spray:
                    transform.position +=
                        projectile.direction * projectile.speed * dt;
                    break;
            }

            // Collision check with player
            if (check_collision(transform.position,
                                player_transform.position)) {
                registry.destroy(entity);  // Destroy projectile on hit
                player_health.take_damage(1);
                Sound::play(asset_manager_->get_sound("retro_hurt"), 0.5f);
            }
            projectile.currtime += dt;
            if (projectile.currtime >= projectile.lifetime) {
                registry.destroy(entity);
            }
        });
}

void ProjectileSystem::spawn_horizontal_projectile(bool from_left, float height,
                                                   entt::registry &registry) {
    auto projectile = registry.create();
    glm::vec2 position =
        from_left ? glm::vec2{-3.0f, height} : glm::vec2{3.0f, height};
    glm::vec2 direction =
        from_left ? glm::vec2{1.0f, 0.0f} : glm::vec2{-1.0f, 0.0f};
    registry.emplace<TransformComponent>(projectile, position,
                                         glm::vec2{0.025f, 0.025f});
    registry.emplace<ProjectileComponent>(
        projectile, ProjectileType::Horizontal, direction, 0.5f, 100.0f);
    registry.emplace<RenderComponent>(
        projectile,
        RenderComponent::from_vertices_color(hookline::get_basic_shape_debug(),
                                             {0.74, 0.08, 0.14, 1.0}));
}

void ProjectileSystem::spawn_chasing_projectile(glm::vec2 origin,
                                                entt::registry &registry) {
    auto projectile = registry.create();
    registry.emplace<TransformComponent>(projectile, origin,
                                         glm::vec2{0.025f, 0.025f});
    registry.emplace<ProjectileComponent>(projectile, ProjectileType::Chasing,
                                          glm::vec2{0.0f, 1.0f}, 0.4f, 10.0f);
    registry.emplace<RenderComponent>(
        projectile,
        RenderComponent::from_vertices_color(hookline::get_basic_shape_debug(),
                                             {0.74, 0.08, 0.14, 1.0}));
}

void ProjectileSystem::spawn_spray_projectile(glm::vec2 origin,
                                              entt::registry &registry) {
    for (int i = 0; i < 6; ++i) {
        float angle =
            glm::radians(i * 60.0f);  // 60-degree increments for six directions
        glm::vec2 direction = glm::vec2(cos(angle), sin(angle));
        auto projectile = registry.create();
        registry.emplace<TransformComponent>(projectile, origin,
                                             glm::vec2{0.025f, 0.025f});
        registry.emplace<ProjectileComponent>(projectile, ProjectileType::Spray,
                                              direction, 0.6f, 10.0f);
        registry.emplace<RenderComponent>(
            projectile,
            RenderComponent::from_vertices_color(
                hookline::get_basic_shape_debug(), {0.74, 0.08, 0.14, 1.0}));
    }
}

bool ProjectileSystem::check_collision(const glm::vec2 &projectile_pos,
                                       const glm::vec2 &player_pos) {
    return glm::distance(projectile_pos, player_pos) < 0.08f;
}