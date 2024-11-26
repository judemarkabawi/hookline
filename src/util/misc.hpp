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
}  // namespace hookline