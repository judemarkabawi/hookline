#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <optional>

namespace hookline {
/**
 * Assume the position is the center of the square and the size is the full
 * width/height
 */
bool point_in_rect(glm::vec2 point, glm::vec2 rect_position,
                   glm::vec2 rect_size);

/**
 * Line-Rectangle detection (adapted)
 * Main Source:
 * https://www.jeffreythompson.org/collision-detection/line-rect.php
 */
std::optional<glm::vec2> line_intersects_rect(glm::vec2 start, glm::vec2 end,
                                              glm::vec2 rect_position,
                                              glm::vec2 rect_size);

/**
 * Check for a ray hit on any colliders
 */
std::optional<glm::vec2> raycast(glm::vec2 start, glm::vec2 direction,
                                 float max_length, entt::registry& registry,
                                 const std::vector<entt::entity>& ignore_list);

/**
 * Convert mouse pixel coordinates to the OpenGL (-1, 1) coordinate system
 *
 * TODO: Move out of physics util
 */
glm::vec2 convert_mouse_to_opengl(int x, int y, glm::uvec2 drawable_size);

/**
 * Given a coordinate in OpenGL coordinates and the properties of the camera,
 * find the world location of the coordinate.
 *
 * TODO: Move out of physics util
 */
glm::vec2 convert_opengl_mouse_to_world(glm::vec2 opengl_position,
                                        glm::vec2 camera_position,
                                        glm::vec2 camera_viewport_size,
                                        float pixels_per_unit);

}  // namespace hookline