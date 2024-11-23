#pragma once

#include <GL/glew.h>

class CyberpunkBackgroundShader {
   public:
    // Constructor/destructor
    CyberpunkBackgroundShader();
    ~CyberpunkBackgroundShader();

    // Copy operators - can't copy a shader program
    CyberpunkBackgroundShader(const CyberpunkBackgroundShader& other) = delete;
    CyberpunkBackgroundShader& operator=(
        const CyberpunkBackgroundShader& other) = delete;

    // Move operators
    CyberpunkBackgroundShader(CyberpunkBackgroundShader&& other) noexcept;
    CyberpunkBackgroundShader& operator=(
        CyberpunkBackgroundShader&& other) noexcept;

    struct {
        GLuint program = 0;

        // Attribute (per-vertex variable) locations
        GLuint a_position_loc = 0;

        // Uniform locations
        // -- Fragment shader
        GLuint u_time_loc = 0;
        GLuint u_drawable_size_loc = 0;
    } m;
};