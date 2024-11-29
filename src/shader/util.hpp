#pragma once

#include <string>

struct ShaderSource {
    std::string vertex_source;
    std::string fragment_source;
};

namespace hookline {
/**
 * Loads a vertex shader given by the path `vertex_name` and fragment
 * shader given by the path `fragment_name` relative to the shaders
 * asset directory (`"../../assets/shaders/"` relative to the executable).
 */
ShaderSource load_shader_file(const std::string& vertex_name,
                              const std::string& fragment_name);
}  // namespace hookline