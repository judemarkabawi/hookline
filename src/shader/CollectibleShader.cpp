#include "CollectibleShader.hpp"

#include "shader/util.hpp"
#include "util/gl_compile_program.hpp"

CollectibleShader::CollectibleShader() {
    ShaderSource shader_source = hookline::load_shader_file(
        "BasicMeshShader.vert", "CollectibleShader.frag");
    program = gl_compile_program(shader_source.vertex_source,
                                 shader_source.fragment_source);

    a_position_loc = glGetAttribLocation(program.program, "a_position");
    a_texture_coord_loc =
        glGetAttribLocation(program.program, "a_texture_coord");
    a_color_loc = glGetAttribLocation(program.program, "a_color");

    u_position_loc = glGetUniformLocation(program.program, "u_position");
    u_scale_loc = glGetUniformLocation(program.program, "u_scale");
    u_rotation_loc = glGetUniformLocation(program.program, "u_rotation");

    u_camera_position_loc =
        glGetUniformLocation(program.program, "u_camera_position");
    u_camera_viewport_size_loc =
        glGetUniformLocation(program.program, "u_camera_viewport_size");
    u_camera_pixels_per_unit_loc =
        glGetUniformLocation(program.program, "u_camera_pixels_per_unit");

    u_time_loc = glGetUniformLocation(program.program, "u_time");
    u_drawable_size_loc =
        glGetUniformLocation(program.program, "u_drawable_size");
    u_size_ratio_loc = glGetUniformLocation(program.program, "u_size_ratio");
}

void CollectibleShader::updateUniforms(glm::vec2 user_pos, glm::vec2 u_scale,
                                       float u_rotation, glm::vec2 camera_pos,
                                       glm::vec2 camera_viewport_size,
                                       float camera_pixels_per_unit,
                                       glm::vec2 drawable_size, float u_time,
                                       float size_ratio) {
    // Uniforms
    // -- Vertex shader
    glUniform2f(u_position_loc, user_pos.x, user_pos.y);
    glUniform2f(u_scale_loc, u_scale.x, u_scale.y);
    glUniform1f(u_rotation_loc, u_rotation);

    glUniform2f(u_camera_position_loc, camera_pos.x, camera_pos.y);
    glUniform2f(u_camera_viewport_size_loc, camera_viewport_size.x,
                camera_viewport_size.y);
    glUniform1f(u_camera_pixels_per_unit_loc, camera_pixels_per_unit);

    // -- Fragment shader
    glUniform2f(u_drawable_size_loc, drawable_size.x, drawable_size.y);
    glUniform1f(u_time_loc, u_time);
    glUniform1f(u_size_ratio_loc, size_ratio);
}