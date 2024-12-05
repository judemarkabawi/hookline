/**
 * @file BasicMeshShader.hpp
 *
 * GL Shader program for 2D meshes with per-vertex color, or a single texture.
 */

#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>

struct WallShader {
    WallShader();
    ~WallShader();

    // Cannot copy a shader program
    WallShader(const WallShader& other) = delete;
    WallShader& operator=(const WallShader& other) = delete;

    WallShader(WallShader&& other) noexcept;

    WallShader& operator=(WallShader&& other) noexcept;

    struct {
        GLuint program = 0;

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
        GLuint u_frag_texture_loc = 0;
        GLuint u_frag_use_texture_loc = 0;
        GLuint u_time_loc = 0;
        GLuint u_resolution = 0;

    } m;

    // call after binding vertex arrays
    void updateUniforms(glm::vec2 user_pos, glm::vec2 u_scale, float u_rotation,
                        glm::vec2 camera_pos, glm::vec2 camera_viewport_size,
                        float camera_pixels_per_unit, int frag_use_texture,
                        GLuint texture, glm::vec2 resolution);
};