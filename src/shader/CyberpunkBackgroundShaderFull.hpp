#pragma once

#include <GL/glew.h>

class CyberpunkBackgroundShaderFull {
   public:
    // Constructor/destructor
    CyberpunkBackgroundShaderFull();
    ~CyberpunkBackgroundShaderFull();

    // Copy operators - can't copy a shader program
    CyberpunkBackgroundShaderFull(const CyberpunkBackgroundShaderFull& other) =
        delete;
    CyberpunkBackgroundShaderFull& operator=(
        const CyberpunkBackgroundShaderFull& other) = delete;

    // Move operators
    CyberpunkBackgroundShaderFull(
        CyberpunkBackgroundShaderFull&& other) noexcept;
    CyberpunkBackgroundShaderFull& operator=(
        CyberpunkBackgroundShaderFull&& other) noexcept;

    struct {
        GLuint program = -1U;

        // Attribute (per-vertex variable) locations
        GLuint a_position_loc = -1U;

        // Uniform locations
        // -- Fragment shader
        GLuint u_time_loc = -1U;
        GLuint u_drawable_size_loc = -1U;
        GLuint u_camera_pos = -1U;

        // texture locations
        GLuint u_bg_emission = -1U;
        GLuint u_bg_color = -1U;
        GLuint u_bg_normals = -1U;
        GLuint u_mg_emission = -1U;
        GLuint u_mg_color = -1U;
        GLuint u_mg_normals = -1U;
        GLuint u_fg_emission = -1U;
        GLuint u_fg_color = -1U;
        GLuint u_fg_normals = -1U;
        GLuint u_bg_cube = -1U;
    } m;
};
