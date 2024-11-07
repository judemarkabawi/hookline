#include "CollectableSystem.hpp"

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <iostream>

#include "CollectableComponent.hpp"
#include "core/TransformComponent.hpp"
#include "render/RenderComponent.hpp"
#include "util/misc.hpp"

CollectableSystem::CollectableSystem(AssetManager *asset_manager)
    : asset_manager_(asset_manager) {}

void CollectableSystem::update(float dt, entt::registry &registry,
                               entt::entity player) {
    (void)dt;
    auto player_position = registry.get<TransformComponent>(player).position;
    auto view = registry.view<TransformComponent, CollectableComponent>();
    std::vector<entt::entity> to_pickup;
    for (auto [entity, transform, collectable] : view.each()) {
        if (glm::distance(transform.position, player_position) <= 0.1f) {
            to_pickup.push_back(entity);
        }
    }
    for (auto entity : to_pickup) {
        on_pickup(registry, entity);
    }
}

void CollectableSystem::spawn(entt::registry &registry, glm::vec2 position) {
    auto collectable = registry.create();
    registry.emplace<TransformComponent>(
        collectable,
        TransformComponent(position, glm::vec2{0.025f, 0.025f}, 0.0f));
    registry.emplace<CollectableComponent>(collectable);
    registry.emplace<RenderComponent>(
        collectable,
        RenderComponent::from_vertices_color(hookline::get_basic_shape_debug(),
                                             {0.96, 0.48, 0.16, 1.0}));

    std::cout << "Spawning collectable " << static_cast<uint32_t>(collectable)
              << " with position " << "(" << position.x << ", " << position.y
              << ")\n";
}

void CollectableSystem::spawn_random(entt::registry &registry) {
    glm::vec2 random_position =
        glm::linearRand(glm::vec2{-2.5f, -2.5f}, glm::vec2{2.5f, 2.5f});
    spawn(registry, random_position);
}

void CollectableSystem::on_pickup(entt::registry &registry,
                                  entt::entity collectable) {
    std::cout << "Picked up " << static_cast<uint32_t>(collectable) << "\n";
    registry.destroy(collectable);
    // spawn_random(registry);
    score += 1;
    Sound::play(asset_manager_->get_sound("item_pick_up"), 0.5f);
}