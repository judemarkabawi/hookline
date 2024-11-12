#include "ForceComponent.hpp"

#include <glm/glm.hpp>
#include <numeric>

ForceComponent &ForceComponent::set_force(glm::vec2 linear_force) {
    reset();
    add_force(linear_force);
    return *this;
}

ForceComponent &ForceComponent::add_force(glm::vec2 linear_force) {
    linear_forces.push_back(linear_force);
    return *this;
}

glm::vec2 ForceComponent::net_force() {
    return std::accumulate(linear_forces.begin(), linear_forces.end(),
                           glm::vec2{0.0f, 0.0f});
}

ForceComponent &ForceComponent::reset() {
    linear_forces.clear();
    return *this;
}