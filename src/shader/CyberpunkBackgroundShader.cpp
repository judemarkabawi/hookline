#include "CyberpunkBackgroundShader.hpp"

#include "shader/util.hpp"
#include "util/gl_compile_program.hpp"

CyberpunkBackgroundShader::CyberpunkBackgroundShader() {
    ShaderSource shader_source = hookline::load_shader_file(
        "SimplePosition.vert", "CyberpunkBackgroundShader.frag");
    m.program = gl_compile_program(shader_source.vertex_source,
                                   shader_source.fragment_source);

    m.a_position_loc = glGetAttribLocation(m.program, "a_position");
    m.u_time_loc = glGetUniformLocation(m.program, "u_time");
    m.u_drawable_size_loc = glGetUniformLocation(m.program, "u_drawable_size");
}

CyberpunkBackgroundShader::~CyberpunkBackgroundShader() {
    glDeleteProgram(m.program);
}

CyberpunkBackgroundShader::CyberpunkBackgroundShader(
    CyberpunkBackgroundShader&& other) noexcept
    : m(std::move(other.m)) {
    other.m.program = 0;
}

CyberpunkBackgroundShader& CyberpunkBackgroundShader::operator=(
    CyberpunkBackgroundShader&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    // Delete current program
    glDeleteProgram(m.program);

    // Move
    m = std::move(other.m);

    // Reset other
    other.m.program = 0;

    return *this;
}