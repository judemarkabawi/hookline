#include "RigidBodyComponent.hpp"

#include "constants.hpp"

void RigidBodyComponent::clamp_velocity() {
    velocity =
        glm::clamp(velocity, -hookline::max_velocity, hookline::max_velocity);
}

RigidBodyComponent &RigidBodyComponent::with_mass(float mass) {
    this->mass = mass;
    return *this;
}

RigidBodyComponent &RigidBodyComponent::with_bounce(float bounce) {
    this->bounce = bounce;
    return *this;
}

RigidBodyComponent &RigidBodyComponent::with_velocity(glm::vec2 velocity) {
    this->velocity = velocity;
    return *this;
}
