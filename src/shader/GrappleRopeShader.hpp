/**
 * @file BasicMeshShader.hpp
 *
 #include "render/ShaderProgram.hpp"
 * GL Shader program for 2D meshes with per-vertex color, or a single texture.
 */

#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>

#include "render/ShaderProgram.hpp"

struct GrappleRopeShader {
    GrappleRopeShader();

    ShaderProgram program;

    // Attribute (per-vertex variable) locations
    GLuint a_position_loc = 0;

    GLuint a_texture_coord_loc = 0;
    GLuint a_color_loc = 0;

    // Uniform locations
    GLuint u_position_loc = 0;
    GLuint u_scale_loc = 0;
    GLuint u_rotation_loc = 0;
    GLuint u_camera_position_loc = 0;
    GLuint u_camera_viewport_size_loc = 0;
    GLuint u_camera_pixels_per_unit_loc = 0;
    GLuint u_player_position_loc = 0;
    //   -- Fragment shader
    GLuint u_time_loc = 0;
    GLuint u_resolution = 0;

    // call after binding vertex arrays
    void updateUniforms(glm::vec2 user_pos, glm::vec2 u_scale, float u_rotation,
                        glm::vec2 camera_pos, glm::vec2 camera_viewport_size,
                        float camera_pixels_per_unit, float u_time,
                        glm::vec2 resolution);
};