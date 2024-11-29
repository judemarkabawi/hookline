#include "CollisionSystem.hpp"

#include "core/TransformComponent.hpp"
#include "gameplay/HealthComponent.hpp"
#include "physics/ColliderComponent.hpp"
#include "physics/GrapplingHook.hpp"
#include "physics/RigidBodyComponent.hpp"

namespace {
bool check_collision_aabb(const TransformComponent &transform1,
                          const ColliderComponent &collider1,
                          const TransformComponent &transform2,
                          const ColliderComponent &collider2) {
    // Assume collider.size is a half size in local coordinates
    glm::vec2 half1 = collider1.size * transform1.scale;
    glm::vec2 half2 = collider2.size * transform2.scale;

    // Calculate AABB min and max positions
    glm::vec2 min1 = transform1.position - half1;
    glm::vec2 max1 = transform1.position + half1;

    glm::vec2 min2 = transform2.position - half2;
    glm::vec2 max2 = transform2.position + half2;

    // Check for overlap
    return (min1.x < max2.x && max1.x > min2.x && min1.y < max2.y &&
            max1.y > min2.y);
}

}  // namespace

void CollisionSystem::update(float dt, entt::registry &registry) {
    (void)dt;
    auto view =
        registry
            .view<TransformComponent, ColliderComponent, RigidBodyComponent>();
    std::vector<entt::entity> to_destroy;
    for (auto [entity1, transform1, collider1, rigid_body1] : view.each()) {
        for (auto [entity2, transform2, collider2, rigid_body2] : view.each()) {
            if (entity1 == entity2) continue;
            if (check_collision_aabb(transform1, collider1, transform2,
                                     collider2)) {
                if (collider1.can_collide && collider2.can_collide) {
                    handle_collision(entity1, entity2, registry, to_destroy);
                }
            }
        }
    }
    for (auto entity : to_destroy) {
        if (registry.valid(entity)) {
            registry.destroy(entity);
        }
    }
};

/**
 * Basic collision resolution algorithm adapted from various sources
 * TODO: Make this better - bouncing and sliding and more than AABB
 */
void CollisionSystem::handle_collision(entt::entity entity1,
                                       entt::entity entity2,
                                       entt::registry &registry,
                                       std::vector<entt::entity> &to_destroy) {
    auto [collider1, transform1, rigid_body1] =
        registry.get<ColliderComponent, TransformComponent, RigidBodyComponent>(
            entity1);
    auto [collider2, transform2, rigid_body2] =
        registry.get<ColliderComponent, TransformComponent, RigidBodyComponent>(
            entity2);

    glm::vec2 delta = transform2.position - transform1.position;
    glm::vec2 overlap = (collider1.size * transform1.scale +
                         collider2.size * transform2.scale) -
                        glm::abs(delta);

    if (!collider1.can_move && !collider2.can_move) {
        return;
    }

    glm::vec2 normal;

    if (overlap.x < overlap.y) {
        // Horizontal resolution
        if (delta.x > 0) {
            // Object 1 on left, object 2 on right
            if (collider1.can_move) {
                transform1.position.x -= overlap.x;
            } else {
                transform2.position.x += overlap.x;
            }
        } else {
            // Object 1 on right, object 2 on left
            if (collider1.can_move) {
                transform1.position.x += overlap.x;
            } else {
                transform2.position.x -= overlap.x;
            }
        }
        normal = glm::vec2(1.0f, 0.0f);
    } else {
        // Vertical resolution
        if (delta.y > 0) {
            // Object 1 on bottom, object 2 on top
            if (collider1.can_move) {
                transform1.position.y -= overlap.y;
            } else {
                transform2.position.y += overlap.y;
            }
        } else {
            // Object 1 on top, object 2 on bottom
            if (collider1.can_move) {
                transform1.position.y += overlap.y;
            } else {
                transform2.position.y -= overlap.y;
            }
        }
        normal = glm::vec2(0.0f, 1.0f);
    }

    // Velocity adjustment
    glm::vec2 velocity1_normal =
        glm::dot(rigid_body1.velocity, normal) * normal;
    glm::vec2 velocity1_tangent = rigid_body1.velocity - velocity1_normal;

    glm::vec2 velocity2_normal =
        glm::dot(rigid_body2.velocity, normal) * normal;
    glm::vec2 velocity2_tangent = rigid_body2.velocity - velocity2_normal;

    float bounce = glm::min(rigid_body1.bounce, rigid_body2.bounce);
    if (collider1.can_move) {
        rigid_body1.velocity = -bounce * velocity1_normal + velocity1_tangent;
    }
    if (collider2.can_move) {
        rigid_body2.velocity = -bounce * velocity2_normal + velocity2_tangent;
    }

    if (collider1.is_breakable) {
        to_destroy.push_back(entity1);
        // Detach grappling hook if attached
        for (auto [_, grapple] :
             registry.view<GrapplingHookComponent>().each()) {
            if (grapple.attached) {
                grapple.detach();
            }
        }
    }

    if (collider2.is_breakable) {
        to_destroy.push_back(entity2);
        // Detach grappling hook if attached
        for (auto [_, grapple] :
             registry.view<GrapplingHookComponent>().each()) {
            if (grapple.attached) {
                grapple.detach();
            }
        }
    }

    if (collider1.is_damaging && registry.all_of<HealthComponent>(entity2)) {
        auto &health = registry.get<HealthComponent>(entity2);
        health.take_damage(1);
    }
    if (collider2.is_damaging && registry.all_of<HealthComponent>(entity1)) {
        auto &health = registry.get<HealthComponent>(entity1);
        health.take_damage(1);
    }
}
