#include "TextShader.hpp"

#include "shader/util.hpp"
#include "util/gl_compile_program.hpp"

TextShader::TextShader() {
    ShaderSource shader_source =
        hookline::load_shader_file("TextShader.vert", "TextShader.frag");
    m.program = gl_compile_program(shader_source.vertex_source,
                                   shader_source.fragment_source);

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
