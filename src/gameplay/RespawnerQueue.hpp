#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "CollectableComponent.hpp"

#include <queue>
#include <tuple>
#include <chrono>


class RespawnerQueue {
public:
    explicit RespawnerQueue() = default;
    using RespawnTask = std::tuple<std::chrono::steady_clock::time_point, glm::vec2, CollectableType>;

    void add_respawn(glm::vec2 position, CollectableType type, float delay_seconds);
    std::vector<RespawnTask> process_ready();

private:
    struct Compare {
        bool operator()(const RespawnTask &a, const RespawnTask &b) {
            return std::get<0>(a) > std::get<0>(b);
        }
    };

    std::priority_queue<RespawnTask, std::vector<RespawnTask>, Compare> queue_;
};