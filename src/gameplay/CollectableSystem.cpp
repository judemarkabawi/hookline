#include "CollectableSystem.hpp"

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

#include "HealthComponent.hpp"
#include "CollectableComponent.hpp"
#include "physics/RigidBodyComponent.hpp"
#include "physics/ColliderComponent.hpp"
#include "physics/GrapplingHook.hpp"
#include "core/TransformComponent.hpp"
#include "render/RenderComponent.hpp"
#include "util/misc.hpp"
#include "constants.hpp"

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
        on_pickup(registry, entity, player);
    }
}

void CollectableSystem::spawn(entt::registry &registry, glm::vec2 position, CollectableType type) {
    auto collectable = registry.create();
    registry.emplace<TransformComponent>(
        collectable,
        TransformComponent(position, glm::vec2{0.025f, 0.025f}, 0.0f));
    registry.emplace<CollectableComponent>(collectable, CollectableComponent{type});
    std::vector<glm::vec2> points = hookline::get_basic_shape_debug(1.0 / hookline::collectible_glow_ratio);
    glm::vec4 color;
    switch (type) {
        case CollectableType::Feather:
            color = {0.96, 0.48, 0.16, 1.0};  // Orange for Feather
            break;
        case CollectableType::Potion:
            color = {0.2f, 0.8f, 0.2f, 1.0f};  // Green for Potion
            break;
        default:
            color = {0.96, 0.48, 0.16, 1.0};  
            break;
    }
    registry.emplace<RenderComponent>(
        collectable,
        RenderComponent::from_vertices_color_tex(points, color, 
               hookline::get_basic_uvs_debug(), RenderComponent::RenderType::COLLECTIBLE));

}

void CollectableSystem::spawn_random(entt::registry &registry) {
    glm::vec2 random_position =
        glm::linearRand(glm::vec2{-2.5f, -2.5f}, glm::vec2{2.5f, 2.5f});
    spawn(registry, random_position, CollectableType::Feather);
}

void CollectableSystem::on_pickup(entt::registry &registry,
                                  entt::entity collectable, entt::entity player_entity) {

    auto &collectable_component = registry.get<CollectableComponent>(collectable);

    switch (collectable_component.type) {
        case CollectableType::Feather: {

            // Get velocity and boost direction
            auto &player_rigidbody = registry.get<RigidBodyComponent>(player_entity);
            glm::vec2 boost_direction = glm::normalize(player_rigidbody.velocity);

            // Detach grappling hook if attached
            for (auto [_, grapple] : registry.view<GrapplingHookComponent>().each()) {
                if(grapple.attached) {
                    grapple.detach();
                }
            }

            if (glm::length(boost_direction) == 0.0f) {
                boost_direction = glm::vec2(0.0f, 1.0f);  // Default upward boost
            }

            float boost_multiplier = 3.0f;  
            player_rigidbody.velocity += boost_direction * boost_multiplier;

            break;
        }
        case CollectableType::Potion: {
            auto &health = registry.get<HealthComponent>(player_entity);
            health.heal(1);
            break;
        }
        default: {
            score += 1;  // Default score increment
            break;
        }
    }

    registry.destroy(collectable);
    //TODO: refactor different play sounds for different pickups 
    Sound::play(asset_manager_->get_sound("item_pick_up"), 0.5f);
}
