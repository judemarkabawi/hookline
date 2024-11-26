#pragma once

#include <GL/glew.h>

struct TextShader {
    TextShader();
    ~TextShader();

    // Cannot copy a shader program
    TextShader(const TextShader& other) = delete;
    TextShader& operator=(const TextShader& other) = delete;

    TextShader(TextShader&& other) noexcept;
    TextShader& operator=(TextShader&& other) noexcept;

    struct {
        GLuint program = 0;

        // Attribute (per-vertex variable) locations
        GLuint a_position_loc = 0;
        GLuint a_texture_coord_loc = 0;
        GLuint a_color_loc = 0;

        // Uniform locations
        GLuint u_position_loc = 0;

        //   -- Fragment shader
        GLuint u_frag_texture_loc = 0;
    } m;
};