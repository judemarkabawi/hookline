/**
 * @file constants.hpp
 *
 * Useful constants for use throughout the game.
 */

#pragma once

#include <glm/glm.hpp>
namespace hookline {

// General
constexpr int default_window_width = 1920;
constexpr int default_window_height = 1080;
constexpr int minimum_window_width = 100;
constexpr int minimum_window_height = 100;
constexpr float fixed_dt = 1.0f / 60.0f;

constexpr float pixels_per_unit = 400.0f;
constexpr float camera_player_vertical_offset = 0.5;

// Physics
constexpr glm::vec2 min_velocity = {0.05, 0.05};
constexpr glm::vec2 max_velocity = {5.0, 5.0};
constexpr glm::vec2 player_movement_force = {1.0f, 1.0f};

constexpr float g = -2.0f;
constexpr float drag = 1.0f;

// Grapple
constexpr float grapple_max_length = 4.0f;
constexpr float grapple_pull_force = 5.0f;
constexpr float grapple_deactive_distance = 0.01f;

}  // namespace hookline