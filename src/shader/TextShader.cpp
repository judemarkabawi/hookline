#include "TextShader.hpp"

#include "shader/util.hpp"
#include "util/gl_compile_program.hpp"

TextShader::TextShader() {
    ShaderSource shader_source =
        hookline::load_shader_file("TextShader.vert", "TextShader.frag");
    program = gl_compile_program(shader_source.vertex_source,
                                 shader_source.fragment_source);

    a_position_loc = glGetAttribLocation(program.program, "a_position");
    a_texture_coord_loc =
        glGetAttribLocation(program.program, "a_texture_coord");
    a_color_loc = glGetAttribLocation(program.program, "a_color");
    u_position_loc = glGetUniformLocation(program.program, "u_position");
}
