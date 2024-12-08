#include "CyberpunkBackgroundShader.hpp"

#include "shader/util.hpp"
#include "util/gl_compile_program.hpp"

CyberpunkBackgroundShader::CyberpunkBackgroundShader() {
    ShaderSource shader_source = hookline::load_shader_file(
        "SimplePosition.vert", "CyberpunkBackgroundShader.frag");
    program = gl_compile_program(shader_source.vertex_source,
                                 shader_source.fragment_source);

    a_position_loc = glGetAttribLocation(program.program, "a_position");
    u_time_loc = glGetUniformLocation(program.program, "u_time");
    u_drawable_size_loc =
        glGetUniformLocation(program.program, "u_drawable_size");
}
