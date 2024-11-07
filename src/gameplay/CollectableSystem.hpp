#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "core/AssetManager.hpp"

class CollectableSystem {
   public:
    explicit CollectableSystem() = default;
    explicit CollectableSystem(AssetManager *asset_manager);

    void update(float dt, entt::registry &registry, entt::entity player);
    void spawn(entt::registry &registry, glm::vec2 position);
    void spawn_random(entt::registry &registry);
    void on_pickup(entt::registry &registry, entt::entity collectable);

    int score = 0;

   private:
    AssetManager *asset_manager_;
};