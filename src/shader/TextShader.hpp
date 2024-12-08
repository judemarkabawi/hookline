#pragma once

#include <GL/glew.h>

#include "render/ShaderProgram.hpp"

struct TextShader {
    TextShader();

    ShaderProgram program;

    // Attribute (per-vertex variable) locations
    GLuint a_position_loc = 0;
    GLuint a_texture_coord_loc = 0;
    GLuint a_color_loc = 0;

    // Uniform locations
    GLuint u_position_loc = 0;

    //   -- Fragment shader
    GLuint u_frag_texture_loc = 0;
};