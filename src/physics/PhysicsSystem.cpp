#include "PhysicsSystem.hpp"

#include "constants.hpp"
#include "core/InputComponent.hpp"
#include "core/TransformComponent.hpp"
#include "physics/ForceComponent.hpp"
#include "physics/GrapplingHook.hpp"
#include "physics/RigidBodyComponent.hpp"

constexpr glm::vec2 g_accel() { return {0.0f, hookline::g}; }
void PhysicsSystem::update(float dt, entt::registry& registry) {
    /* Grappling hook physics */
    for (auto [_, grapple] : registry.view<GrapplingHookComponent>().each()) {
        if (grapple.attached) {
            auto [player_transform, player_forces, player_rigidBody] =
                registry.get<TransformComponent, ForceComponent,
                             RigidBodyComponent>(grapple.user);

            // Add pulling force
            glm::vec2 grapple_direction = glm::normalize(
                grapple.attached_position - player_transform.position);
            if (grapple.held) {
                float cosTheta = -grapple_direction.y;
                float r = glm::length(grapple.attached_position - player_transform.position);
                glm::vec2 tanDir = glm::vec2(-grapple_direction.y, grapple_direction.x);
                float v = glm::dot(player_rigidBody.velocity, tanDir);
                float centripetalAccel = (v*v)/r;
                float tension = player_rigidBody.mass * (hookline::g * cosTheta + centripetalAccel);
                player_forces.add_force(grapple_direction * tension);
            } else if (grapple.attached)
                player_forces.add_force(grapple_direction * grapple.pull_force);

            // Deactive grapple when close
            if (glm::distance(player_transform.position,
                              grapple.attached_position) <
                grapple.deactivate_distance) {
                grapple.detach();
            }
        }
    }
    // Physics update
    auto view =
        registry.view<RigidBodyComponent, ForceComponent, TransformComponent>();
    for (auto [entity, rigid_body, forces, transform] : view.each()) {
        forces.add_force(rigid_body.mass * g_accel());
        glm::vec2 force = forces.net_force();

        if (registry.all_of<InputComponent>(entity)) {
            // Player
            force += registry.get<InputComponent>(entity).movement;
        }

        rigid_body.velocity += (force / rigid_body.mass) * dt;
        rigid_body.velocity -= hookline::drag * rigid_body.velocity * dt;
        rigid_body.clamp_velocity();
        transform.position += rigid_body.velocity * dt;

        forces.reset();
    }
}