#include "CollisionSystem.hpp"

#include "core/TransformComponent.hpp"
#include "physics/ColliderComponent.hpp"
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
    for (auto [entity1, transform1, collider1, rigid_body1] : view.each()) {
        for (auto [entity2, transform2, collider2, rigid_body2] : view.each()) {
            if (entity1 == entity2) continue;
            if (check_collision_aabb(transform1, collider1, transform2,
                                     collider2)) {
                if (collider1.can_collide && collider2.can_collide) {
                    handle_collision(entity1, entity2, registry);
                }
            }
        }
    }
};

/**
 * Basic collision resolution algorithm adapted from various sources
 * TODO: Make this better - bouncing and sliding and more than AABB
 */
void CollisionSystem::handle_collision(entt::entity entity1,
                                       entt::entity entity2,
                                       entt::registry &registry) {
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

    if (overlap.x < overlap.y) {
        if (delta.x > 0 && collider1.can_move) {
            transform1.position.x -= overlap.x;
        } else if (collider2.can_move) {
            transform2.position.x += collider2.can_move * overlap.x;
        }
    } else {
        if (delta.y > 0 && collider1.can_move) {
            transform1.position.y -= overlap.y;
        } else if (collider2.can_move) {
            transform2.position.y += collider2.can_move * overlap.y;
        }
    }

    // Turn off bouncing and sliding
    rigid_body1.velocity *= !collider1.can_move;
    rigid_body2.velocity *= !collider2.can_move;
}
