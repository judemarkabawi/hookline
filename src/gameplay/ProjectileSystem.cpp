#include "ProjectileSystem.hpp"

#include <cmath>
#include <random>

#include "core/TransformComponent.hpp"
#include "gameplay/HealthComponent.hpp"
#include "gameplay/ProjectileComponent.hpp"
#include "gameplay/ProjectileSpawnerComponent.hpp"
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
    // Process all spawners
    registry.view<TransformComponent, ProjectileSpawnerComponent>().each(
        [&](auto entity, auto &transform, auto &spawner) {
            spawner.timer += dt;

            switch (spawner.type) {
                case SpawnerType::Horizontal:
                    if (spawner.timer >= spawner.interval) {
                        spawner.timer = -1000.f;
                        spawn_horizontal_projectile(
                            transform.position.y, spawner.position_range,
                            spawner.projectile_speed, spawner.projectile_lifetime, registry);
                    }
                    break;

                case SpawnerType::Vertical:
                    if (spawner.timer >= spawner.interval) {
                        spawner.timer = -1000.f;
                        spawn_vertical_projectile(
                            transform.position.x, spawner.position_range,
                            spawner.projectile_speed, spawner.projectile_lifetime, registry);
                    }
                    break;

                case SpawnerType::Chasing:
                    if (player_transform.position.y >= spawner.trigger_height) {
                        if (spawner.timer >= spawner.interval) {
                            spawner.timer = 0;
                            spawner.shots_remaining = spawner.burst_count;
                        }

                        if (spawner.shots_remaining > 0 &&
                            spawner.timer >= spawner.small_interval) {
                            spawner.timer -= spawner.small_interval;
                            --spawner.shots_remaining;

                            spawn_chasing_projectile(
                                transform.position, spawner.projectile_speed,
                                spawner.projectile_lifetime, registry);
                        }
                    }
                    break;

                case SpawnerType::Spray:
                    if (player_transform.position.y >= spawner.trigger_height) {
                        if (spawner.timer >= spawner.interval) {
                            spawner.timer = 0;
                            spawner.shots_remaining = spawner.burst_count;
                        }

                        if (spawner.shots_remaining > 0 &&
                            spawner.timer >= spawner.small_interval) {
                            spawner.timer -= spawner.small_interval;
                            --spawner.shots_remaining;

                            spawn_spray_projectile(
                                transform.position, spawner.projectile_count,
                                spawner.projectile_speed,
                                spawner.projectile_lifetime, registry);
                        }
                    }
                    break;
            }
        });

    registry.view<TransformComponent, ProjectileComponent>().each(
        [&](auto entity, auto &transform, auto &projectile) {
            projectile.currtime += dt;

            switch (projectile.type) {
                case ProjectileType::Horizontal:
                        transform.position +=
                            projectile.direction * projectile.speed * dt;
                    if (transform.position.x > projectile.range[1] ||
                        transform.position.x < projectile.range[0]) {
                        projectile.direction = -projectile.direction;
                    }
                    break;
                case ProjectileType::Vertical:
                    transform.position +=
                        projectile.direction * projectile.speed * dt;
                    if (transform.position.y > projectile.range[1] ||
                        transform.position.y < projectile.range[0]) {
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
            bool to_destroy = false;
            // Collision check with player
            if (check_collision(transform.position,
                                player_transform.position)) {
                to_destroy = true;  // Destroy projectile on hit
                player_health.take_damage(1);
                Sound::play(asset_manager_->get_sound("retro_hurt"), 0.5f);
            }
            if (projectile.currtime >= projectile.lifetime) {
                to_destroy = true;
            }
            if(to_destroy){
                registry.destroy(entity);
            }
        });
}

void ProjectileSystem::spawn_horizontal_projectile(
                            float height, glm::vec2 position_range,
                            float projectile_speed, float projectile_lifetime, entt::registry &registry)
    {
    auto projectile = registry.create();
    glm::vec2 direction = glm::vec2{1.0f, 0.0f};
    glm::vec2 position = glm::vec2{position_range[0], height};
    registry.emplace<TransformComponent>(projectile, position,
                                         glm::vec2{0.025f, 0.025f});
    registry.emplace<ProjectileComponent>(
        projectile, ProjectileType::Horizontal, direction, projectile_speed, projectile_lifetime, position_range, height);
    registry.emplace<RenderComponent>(
        projectile,
        RenderComponent::from_vertices_color(hookline::get_basic_shape_debug(),
                                             {0.74, 0.08, 0.14, 1.0}));
}


void ProjectileSystem::spawn_vertical_projectile(
                            float x_offset, glm::vec2 position_range,
                            float projectile_speed, float projectile_lifetime, entt::registry &registry)
    {
    auto projectile = registry.create();
    glm::vec2 direction = glm::vec2{0.0f, 1.0f};
    glm::vec2 position = glm::vec2{x_offset, position_range[0]};
    registry.emplace<TransformComponent>(projectile, position,
                                         glm::vec2{0.025f, 0.025f});
    registry.emplace<ProjectileComponent>(
        projectile, ProjectileType::Vertical, direction, projectile_speed, projectile_lifetime, position_range, x_offset);
    registry.emplace<RenderComponent>(
        projectile,
        RenderComponent::from_vertices_color(hookline::get_basic_shape_debug(),
                                             {0.74, 0.08, 0.14, 1.0}));
}

void ProjectileSystem::spawn_chasing_projectile(
                                glm::vec2 position, float projectile_speed,
                                float projectile_lifetime, entt::registry &registry){
    auto projectile = registry.create();
    glm::vec2 direction = glm::vec2{0.0f, 0.0f};
    registry.emplace<TransformComponent>(projectile, position,
                                         glm::vec2{0.025f, 0.025f});
    registry.emplace<ProjectileComponent>(projectile, ProjectileType::Chasing,
                                          direction, projectile_speed, projectile_lifetime, glm::vec2{0.0f, 0.0f}, 0.0f);
    registry.emplace<RenderComponent>(
        projectile,
        RenderComponent::from_vertices_color(hookline::get_basic_shape_debug(),
                                             {0.74, 0.08, 0.14, 1.0}));
}

void ProjectileSystem::spawn_spray_projectile(
    glm::vec2 origin, int projectile_count, float projectile_speed,
    float projectile_lifetime, entt::registry &registry) {
    // Divide the full circle (360 degrees) by the number of projectiles
    float angle_step = 360.0f / projectile_count;

    for (int i = 0; i < projectile_count; ++i) {
        float angle = glm::radians(i * angle_step);  // Compute angle in radians
        glm::vec2 direction = glm::vec2(cos(angle), sin(angle));  // Unit vector

        auto projectile = registry.create();
        registry.emplace<TransformComponent>(
            projectile, origin, glm::vec2{0.025f, 0.025f});
        registry.emplace<ProjectileComponent>(
            projectile, ProjectileType::Spray, direction, projectile_speed,
            projectile_lifetime, glm::vec2{0.0f, 0.0f}, 0.0f);
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