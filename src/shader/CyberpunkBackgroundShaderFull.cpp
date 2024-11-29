#include "CyberpunkBackgroundShaderFull.hpp"

#include "shader/util.hpp"
#include "util/gl_compile_program.hpp"

CyberpunkBackgroundShaderFull::CyberpunkBackgroundShaderFull() {
    ShaderSource shader_source = hookline::load_shader_file(
        "SimplePosition.vert", "CyberpunkBackgroundShaderFull.frag");
    m.program = gl_compile_program(shader_source.vertex_source,
                                   shader_source.fragment_source);

    // positional uniforms
    m.a_position_loc = glGetAttribLocation(m.program, "a_position");
    m.u_time_loc = glGetUniformLocation(m.program, "u_time");
    m.u_drawable_size_loc = glGetUniformLocation(m.program, "u_drawable_size");
    m.u_camera_pos = glGetUniformLocation(m.program, "u_camera_pos");

    // texture uniforms
    m.u_bg_emission = glGetUniformLocation(m.program, "u_bg_emission");
    m.u_bg_color = glGetUniformLocation(m.program, "u_bg_color");
    m.u_bg_normal = glGetUniformLocation(m.program, "u_bg_normal");

    m.u_mg_emission = glGetUniformLocation(m.program, "u_mg_emission");
    m.u_mg_color = glGetUniformLocation(m.program, "u_mg_color");
    m.u_mg_normal = glGetUniformLocation(m.program, "u_mg_normal");

    m.u_fg_emission = glGetUniformLocation(m.program, "u_fg_emission");
    m.u_fg_color = glGetUniformLocation(m.program, "u_fg_color");
    m.u_fg_normal = glGetUniformLocation(m.program, "u_fg_normal");

    m.u_bg_cube = glGetUniformLocation(m.program, "u_bg_cube");

    // set TEX to always refer to texture binding zero:
    glUseProgram(m.program);  // bind program -- glUniform* calls refer to this
                              // program now

    glUniform1i(m.u_bg_emission, 0);
    glUniform1i(m.u_bg_color, 1);
    glUniform1i(m.u_bg_normal, 2);
    glUniform1i(m.u_mg_emission, 3);
    glUniform1i(m.u_mg_color, 4);
    glUniform1i(m.u_mg_normal, 5);
    glUniform1i(m.u_fg_emission, 6);
    glUniform1i(m.u_fg_color, 7);
    glUniform1i(m.u_fg_normal, 8);
    glUniform1i(m.u_bg_cube, 9);

    glUseProgram(0);  // unbind program -- glUniform* calls refer to ??? now
}

CyberpunkBackgroundShaderFull::~CyberpunkBackgroundShaderFull() {
    glDeleteProgram(m.program);
}

CyberpunkBackgroundShaderFull::CyberpunkBackgroundShaderFull(
    CyberpunkBackgroundShaderFull&& other) noexcept
    : m(std::move(other.m)) {
    other.m.program = 0;
}

CyberpunkBackgroundShaderFull& CyberpunkBackgroundShaderFull::operator=(
    CyberpunkBackgroundShaderFull&& other) noexcept {
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