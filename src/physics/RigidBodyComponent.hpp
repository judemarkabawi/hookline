#pragma once

#include <glm/glm.hpp>

/**
 * Component representing information about a Rigid Body.
 */
struct RigidBodyComponent {
    float mass = 1.0f;
    /** Bounce is like a coefficient of restitution, should be 0.0 to 1.0 */
    float bounce = 0.6f;
    glm::vec2 velocity = {0.0f, 0.0f};

    void clamp_velocity();

    // Builders
    RigidBodyComponent &with_mass(float mass);
    RigidBodyComponent &with_bounce(float bounce);
    RigidBodyComponent &with_velocity(glm::vec2 velocity);
};
