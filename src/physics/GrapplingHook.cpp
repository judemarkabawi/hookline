#include "GrapplingHook.hpp"

#include "core/TransformComponent.hpp"
#include "physics/RigidBodyComponent.hpp"
#include "physics/util.hpp"

void GrapplingHookComponent::try_attach(glm::vec2 start_position,
                                        glm::vec2 target_position,
                                        entt::registry &registry) {
    if (attached) {
        return;
    }

    glm::vec2 direction = glm::normalize(target_position - start_position);
    std::vector<entt::entity> ignore_list = {user, self};
    auto maybe_hit = hookline::raycast(start_position, direction, max_length,
                                       registry, ignore_list);
    if (maybe_hit.has_value()) {
        attached = true;
        attached_position = maybe_hit.value();
    }
}

void GrapplingHookComponent::detach() {
    if (!attached) {
        return;
    }
    attached = false;
    held = false;
}

void GrapplingHookComponent::hold(entt::registry &registry) {
    if (!attached) return;
    held = true;
    auto [player_transform, player_rigidBody] =
        registry.get<TransformComponent, RigidBodyComponent>(user);

    glm::vec2 grapple_direction =
        glm::normalize(attached_position - player_transform.position);
    glm::vec2 projVelocity =
        glm::dot(player_rigidBody.velocity, grapple_direction) *
        grapple_direction;
    player_rigidBody.velocity -= 1.0f * projVelocity;
}

void GrapplingHookComponent::unhold() {
    if (!attached) return;
    held = false;
}

glm::vec2 GrapplingHookComponent::get_left_dir(glm::vec2 playerPos) {
    glm::vec2 grappleDir = glm::normalize(attached_position - playerPos);
    return glm::vec2(-grappleDir.y, grappleDir.x);
}
glm::vec2 GrapplingHookComponent::get_right_dir(glm::vec2 playerPos) {
    glm::vec2 grappleDir = glm::normalize(attached_position - playerPos);
    return glm::vec2(grappleDir.y, -grappleDir.x);
}
glm::vec2 GrapplingHookComponent::get_up_dir(glm::vec2 playerPos) {
    return glm::normalize(attached_position - playerPos);
}
glm::vec2 GrapplingHookComponent::get_down_dir(glm::vec2 playerPos) {
    return glm::normalize(playerPos - attached_position);
}

GrapplingHookComponent::GrapplingHookComponent(entt::entity self_,
                                               entt::entity user_)
    : self(self_), user(user_) {}