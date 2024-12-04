#include "CollectableSystem.hpp"
#include <iostream>
#include <glm/geometric.hpp>

void RespawnerQueue::add_respawn(glm::vec2 position, CollectableType type, float delay_seconds) {
    auto respawn_time = std::chrono::steady_clock::now() + std::chrono::milliseconds(static_cast<int>(delay_seconds * 1000));
    queue_.emplace(respawn_time, position, type);
}

std::vector<RespawnerQueue::RespawnTask> RespawnerQueue::process_ready() {
    std::vector<RespawnTask> ready_tasks;
    auto now = std::chrono::steady_clock::now();
    while (!queue_.empty() && std::get<0>(queue_.top()) <= now) {
        ready_tasks.push_back(queue_.top());
        queue_.pop();
    }
    return ready_tasks;
}
