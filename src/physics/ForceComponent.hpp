#pragma once

#include <glm/glm.hpp>
#include <vector>

/**
 * Component attached to an entity to represent forces applied to it.
 *
 * TODO: preallocate memory for common forces
 *
 * TODO: rotational forces?
 */
struct ForceComponent {
    /**
     * TODO: revisit - is there ever a reason to need these split up or can we
     * just accumulate forces and only store a net? Kept this way for now in case
     */
    std::vector<glm::vec2> linear_forces;

    /**
     * Set the total force.
     */
    ForceComponent &set_force(glm::vec2 linear_force);

    /**
     * Add a force to forces on an object.
     */
    ForceComponent &add_force(glm::vec2 linear_force);

    /**
     * Get the net force on an object.
     */
    glm::vec2 net_force();

    /**
     * Reset all forces.
     * Equivalent to `set_force({0.0f, 0.0f})`.
     */
    ForceComponent &reset();
};
