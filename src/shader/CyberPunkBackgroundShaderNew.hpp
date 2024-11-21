#pragma once

#include <GL/glew.h>

class CyberpunkBackgroundShaderNew {
   public:
    // Constructor/destructor
    CyberpunkBackgroundShaderNew();
    ~CyberpunkBackgroundShaderNew();

    // Copy operators - can't copy a shader program
    CyberpunkBackgroundShaderNew(const CyberpunkBackgroundShaderNew& other) = delete;
    CyberpunkBackgroundShaderNew& operator=(
        const CyberpunkBackgroundShaderNew& other) = delete;

    // Move operators
    CyberpunkBackgroundShaderNew(CyberpunkBackgroundShaderNew&& other) noexcept;
    CyberpunkBackgroundShaderNew& operator=(
        CyberpunkBackgroundShaderNew&& other) noexcept;

    GLuint program = 0;

    // Attribute (per-vertex variable) locations
    GLuint a_position_loc = 0;

    // Uniform locations
    // -- Fragment shader
    GLuint u_time_loc = 0;
    GLuint u_drawable_size_loc = 0;
};