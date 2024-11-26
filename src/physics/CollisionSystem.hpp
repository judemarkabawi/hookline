#pragma once

#include <entt/entt.hpp>

class CollisionSystem {
   public:
    void update(float dt, entt::registry &registry);

   private:
    void handle_collision(entt::entity entity1, entt::entity entity2,
                          entt::registry &registry, std::vector<entt::entity> &to_destroy);
};