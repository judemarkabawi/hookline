#pragma once

#include <GL/glew.h>

#include "render/ShaderProgram.hpp"

class CyberpunkBackgroundShaderFull {
   public:
    CyberpunkBackgroundShaderFull();

    ShaderProgram program;

    // Attribute (per-vertex variable) locations
    GLuint a_position_loc = 0;

    // Uniform locations
    // -- Fragment shader
    GLuint u_time_loc = 0;
    GLuint u_drawable_size_loc = 0;
    GLuint u_camera_pos = 0;

    // texture locations
    GLuint u_bg_emission = 0;
    GLuint u_bg_color = 0;
    GLuint u_bg_normals = 0;
    GLuint u_mg_emission = 0;
    GLuint u_mg_color = 0;
    GLuint u_mg_normals = 0;
    GLuint u_fg_emission = 0;
    GLuint u_fg_color = 0;
    GLuint u_fg_normals = 0;
    GLuint u_bg_cube = 0;
};
