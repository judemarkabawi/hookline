#include "ColliderComponent.hpp"

#include <glm/glm.hpp>

ColliderComponent &ColliderComponent::set_size(glm::vec2 size) {
    this->size = size;
    return *this;
}

ColliderComponent &ColliderComponent::set_can_collide(bool can_collide) {
    this->can_collide = can_collide;
    return *this;
}

ColliderComponent &ColliderComponent::set_can_move(bool can_move) {
    this->can_move = can_move;
    return *this;
}

ColliderComponent &ColliderComponent::set_hookable(bool hookable) {
    this->hookable = hookable;
    return *this;
}

ColliderComponent &ColliderComponent::set_breakable(bool breakable) {
    this->is_breakable = breakable;
    return *this;
}

ColliderComponent &ColliderComponent::set_damaging(bool damaging) {
    this->is_damaging = damaging;
    return *this;
}