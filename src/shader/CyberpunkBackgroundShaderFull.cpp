#include "CyberpunkBackgroundShaderFull.hpp"

#include "shader/util.hpp"
#include "util/gl_compile_program.hpp"

CyberpunkBackgroundShaderFull::CyberpunkBackgroundShaderFull() {
    ShaderSource shader_source = hookline::load_shader_file(
        "SimplePosition.vert", "CyberpunkBackgroundShaderFull.frag");
    program = gl_compile_program(shader_source.vertex_source,
                                 shader_source.fragment_source);

    // positional uniforms
    a_position_loc = glGetAttribLocation(program.program, "a_position");
    u_time_loc = glGetUniformLocation(program.program, "u_time");
    u_drawable_size_loc =
        glGetUniformLocation(program.program, "u_drawable_size");
    u_camera_pos = glGetUniformLocation(program.program, "u_camera_pos");

    // texture uniforms
    u_bg_emission = glGetUniformLocation(program.program, "u_bg_emission");
    u_bg_color = glGetUniformLocation(program.program, "u_bg_color");
    u_bg_normals = glGetUniformLocation(program.program, "u_bg_normal");

    u_mg_emission = glGetUniformLocation(program.program, "u_mg_emission");
    u_mg_color = glGetUniformLocation(program.program, "u_mg_color");
    u_mg_normals = glGetUniformLocation(program.program, "u_mg_normal");

    u_fg_emission = glGetUniformLocation(program.program, "u_fg_emission");
    u_fg_color = glGetUniformLocation(program.program, "u_fg_color");
    u_fg_normals = glGetUniformLocation(program.program, "u_fg_normal");

    u_bg_cube = glGetUniformLocation(program.program, "u_bg_cube");

    // set TEX to always refer to texture binding zero:
    glUseProgram(program.program);  // bind program -- glUniform* calls refer to
                                    // this program now

    glUniform1i(u_bg_emission, 0);
    glUniform1i(u_bg_color, 1);
    glUniform1i(u_bg_normals, 2);
    glUniform1i(u_mg_emission, 3);
    glUniform1i(u_mg_color, 4);
    glUniform1i(u_mg_normals, 5);
    glUniform1i(u_fg_emission, 6);
    glUniform1i(u_fg_color, 7);
    glUniform1i(u_fg_normals, 8);
    glUniform1i(u_bg_cube, 9);

    glUseProgram(0);  // unbind program -- glUniform* calls refer to ??? now
}
