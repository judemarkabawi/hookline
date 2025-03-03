#pragma once

#include <glm/glm.hpp>

/**
 * Component representing information about a Rigid Body.
 */
struct RigidBodyComponent {
    float mass = 1.0f;
    /** Bounce is like a coefficient of restitution, should be 0.0 to 1.0 */
    float bounce = 0.4f;
    glm::vec2 velocity = {0.0f, 0.0f};

    void clamp_velocity();

    // Builders
    RigidBodyComponent &set_mass(float mass);
    RigidBodyComponent &set_bounce(float bounce);
    RigidBodyComponent &set_velocity(glm::vec2 velocity);
};
