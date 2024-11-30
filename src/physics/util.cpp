#include "util.hpp"

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <limits>
#include <optional>

#include "core/TransformComponent.hpp"
#include "physics/ColliderComponent.hpp"

namespace hookline {
bool point_in_rect(glm::vec2 point, glm::vec2 rect_position,
                   glm::vec2 rect_size) {
    return (point.x >= rect_position.x - rect_size.x / 2 &&
            point.x <= rect_position.x + rect_size.x / 2 &&
            point.y >= rect_position.y - rect_size.y / 2 &&
            point.y <= rect_position.y + rect_size.y / 2);
}

std::optional<glm::vec2> line_intersects_rect(glm::vec2 start, glm::vec2 end,
                                              glm::vec2 rect_position,
                                              glm::vec2 rect_size) {
    float left = rect_position.x - rect_size.x;
    float right = rect_position.x + rect_size.x;
    float top = rect_position.y + rect_size.y;
    float bottom = rect_position.y - rect_size.y;

    // Short Circuit - Line is completely to one side of rectangle
    if (start.x <= left && end.x <= left) return std::nullopt;
    if (start.x >= right && end.x >= right) return std::nullopt;
    if (start.y >= top && end.y >= top) return std::nullopt;
    if (start.y <= bottom && end.y <= bottom) return std::nullopt;

    // Check intersections at any edges
    float m = (end.y - start.y) / (end.x - start.x);

    // Bottom edge - technically the most likely
    float x_at_bottom_edge = (bottom - start.y) / m + start.x;
    if (x_at_bottom_edge > left && x_at_bottom_edge < right) {
        return std::make_optional<glm::vec2>(x_at_bottom_edge, bottom);
    }

    // Left edge
    float y_at_left_edge = m * (left - start.x) + start.y;
    if (y_at_left_edge < top && y_at_left_edge > bottom) {
        return std::make_optional<glm::vec2>(left, y_at_left_edge);
    };

    // Right edge
    float y_at_right_edge = m * (right - start.x) + start.y;
    if (y_at_right_edge < top && y_at_right_edge > bottom) {
        return std::make_optional<glm::vec2>(right, y_at_right_edge);
    }

    // Top edge
    float x_at_top_edge = (top - start.y) / m + start.x;
    if (x_at_top_edge > left && x_at_top_edge < right) {
        return std::make_optional<glm::vec2>(x_at_top_edge, top);
    }

    return std::nullopt;
}

std::optional<glm::vec2> raycast(glm::vec2 start, glm::vec2 direction,
                                 float max_length, entt::registry& registry,
                                 const std::vector<entt::entity>& ignore_list) {
    direction = glm::normalize(direction);

    auto view = registry.view<TransformComponent, ColliderComponent>();
    float closest_hit_distance = std::numeric_limits<float>::max();
    std::optional<glm::vec2> closest_hit = std::nullopt;
    for (const auto [entity, transform, collider] : view.each()) {
        // Ignore unhookable and ignored entities
        if (!collider.hookable ||
            std::find(ignore_list.begin(), ignore_list.end(), entity) !=
                ignore_list.end()) {
            continue;
        }
        // Check for ray intersection
        auto maybe_hit = line_intersects_rect(
            start, start + direction * max_length, transform.position,
            collider.size * transform.scale);
        if (maybe_hit.has_value()) {
            float distance = glm::distance(start, maybe_hit.value());
            if (distance <= closest_hit_distance) {
                closest_hit_distance = distance;
                closest_hit = maybe_hit;
            }
        }
    }

    return closest_hit;
}

}  // namespace hookline