#include "TextShader.hpp"

#include "util/gl_compile_program.hpp"

TextShader::TextShader() {
    m.program = gl_compile_program(
        // Vertex shader
        R"(
        #version 330
        layout(location = 0) in vec2 a_position;
        layout(location = 1) in vec2 a_texture_coord;
        layout(location = 2) in vec4 a_color;
        uniform vec2 u_position;  // from transform
        out vec2 texture_coord;

        void main() {
            vec2 world_position = a_position + u_position;
            gl_Position = vec4(world_position, 0.0, 1.0);

            texture_coord = a_texture_coord;
        }
        )",
        // Fragment shader
        R"(
        #version 330
        in vec2 texture_coord;
        uniform sampler2D u_frag_texture;
        out vec4 FragColor;
        
        void main() {
            FragColor = vec4(1.0, 1.0, 1.0, texture(u_frag_texture, texture_coord).r);
        }
        )");

    m.a_position_loc = glGetAttribLocation(m.program, "a_position");
    m.a_texture_coord_loc = glGetAttribLocation(m.program, "a_texture_coord");
    m.a_color_loc = glGetAttribLocation(m.program, "a_color");
    m.u_position_loc = glGetUniformLocation(m.program, "u_position");
}

TextShader::~TextShader() { glDeleteProgram(m.program); }

TextShader::TextShader(TextShader&& other) noexcept : m(other.m) {
    other.m.program = 0;
}

TextShader& TextShader::operator=(TextShader&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    // Delete current program
    glDeleteProgram(m.program);

    // Move from other
    m = std::move(other.m);

    // Reset other
    other.m.program = 0;

    return *this;
}
