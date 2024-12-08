#pragma once

#include <GL/glew.h>

#include "render/ShaderProgram.hpp"

class CyberpunkBackgroundShader {
   public:
    CyberpunkBackgroundShader();

    ShaderProgram program;

    // Attribute (per-vertex variable) locations
    GLuint a_position_loc = 0;

    // Uniform locations
    // -- Fragment shader
    GLuint u_time_loc = 0;
    GLuint u_drawable_size_loc = 0;
};