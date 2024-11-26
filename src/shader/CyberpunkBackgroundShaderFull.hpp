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
    CyberpunkBackgroundShaderFull(CyberpunkBackgroundShaderFull&& other) noexcept;
    CyberpunkBackgroundShaderFull& operator=(
        CyberpunkBackgroundShaderFull&& other) noexcept;

    struct {
        GLuint program = 0;

        // Attribute (per-vertex variable) locations
        GLuint a_position_loc = 0;

        // Uniform locations
        // -- Fragment shader
        GLuint u_time_loc = 0;
        GLuint u_drawable_size_loc = 0;
        GLuint u_camera_pos = 0;

        //texture locations
        GLuint u_bg_emission = 0;
        GLuint u_bg_color = 0;
        GLuint u_bg_normal = 0;
        GLuint u_mg_emission = 0;
        GLuint u_mg_color = 0;
        GLuint u_mg_normal = 0;
        GLuint u_fg_emission = 0;
        GLuint u_fg_color = 0;
        GLuint u_fg_normal = 0;
        GLuint u_bg_cube = 0;
    } m;
};
