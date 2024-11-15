#include "GrapplingHook.hpp"

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
}

GrapplingHookComponent::GrapplingHookComponent(entt::entity self_,
                                               entt::entity user_)
    : self(self_), user(user_) {}