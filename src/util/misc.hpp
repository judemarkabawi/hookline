#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace hookline {
/**
    Get a square with coordinates at the 4 corners in OpenGL coordinates (-1,
   1), etc. from BL -> BR -> TL -> TR.
 */
std::vector<glm::vec2> get_basic_shape_debug(float scale = 1.0);

/**
    Get a square with uvs at the 4 corners in OpenGL coordinates (0,
   1), etc. from BL -> BR -> TL -> TR.
 */
std::vector<glm::vec2> get_basic_uvs_debug();

/**
    Construct a path based on the location of the currently-running executable

    (e.g. if running /home/ix/game0/game.exe will return '/home/ix/game0/' +
   suffix)
 */
std::string data_path(std::string const &suffix);

/**
 * Convert a mouse position from screen (SDL_GetMouseState) coordinates to
 * OpenGL coordinates
 */
glm::vec2 screen_to_opengl(glm::vec2 position, glm::uvec2 drawable_size);

/**
 * Convert a mouse position from OpenGL coordinates to screen coordinates
 */
glm::vec2 opengl_to_screen(glm::vec2 position, glm::uvec2 drawable_size);

glm::vec2 convert_opengl_mouse_to_world(glm::vec2 opengl_position,
                                        glm::vec2 camera_position,
                                        glm::vec2 camera_viewport_size,
                                        float pixels_per_unit);

}  // namespace hookline