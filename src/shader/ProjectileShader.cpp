#include "ProjectileShader.hpp"

#include "shader/util.hpp"
#include "util/gl_compile_program.hpp"

ProjectileShader::ProjectileShader() {
    ShaderSource shader_source = hookline::load_shader_file(
        "BasicMeshShader.vert", "ProjectileShader.frag");
    m.program = gl_compile_program(shader_source.vertex_source,
                                   shader_source.fragment_source);

    m.a_position_loc = glGetAttribLocation(m.program, "a_position");
    m.a_texture_coord_loc = glGetAttribLocation(m.program, "a_texture_coord");
    m.a_color_loc = glGetAttribLocation(m.program, "a_color");

    m.u_position_loc = glGetUniformLocation(m.program, "u_position");
    m.u_scale_loc = glGetUniformLocation(m.program, "u_scale");
    m.u_rotation_loc = glGetUniformLocation(m.program, "u_rotation");

    m.u_camera_position_loc =
        glGetUniformLocation(m.program, "u_camera_position");
    m.u_camera_viewport_size_loc =
        glGetUniformLocation(m.program, "u_camera_viewport_size");
    m.u_camera_pixels_per_unit_loc =
        glGetUniformLocation(m.program, "u_camera_pixels_per_unit");

    m.u_time_loc = glGetUniformLocation(m.program, "u_time");
    m.u_movement_vector_loc =
        glGetUniformLocation(m.program, "u_movement_vector");
    m.u_size_ratio_loc = glGetUniformLocation(m.program, "u_size_ratio");
    m.u_age_loc = glGetUniformLocation(m.program, "u_age");
}

ProjectileShader::~ProjectileShader() { glDeleteProgram(m.program); }

ProjectileShader::ProjectileShader(ProjectileShader&& other) noexcept
    : m(other.m) {
    other.m.program = 0;
}

ProjectileShader& ProjectileShader::operator=(
    ProjectileShader&& other) noexcept {
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

void ProjectileShader::updateUniforms(glm::vec2 user_pos, glm::vec2 u_scale,
                                      float u_rotation, glm::vec2 camera_pos,
                                      glm::vec2 camera_viewport_size,
                                      float camera_pixels_per_unit,
                                      float u_time, glm::vec2 movement_vector,
                                      float size_ratio, float age) {
    // Uniforms
    // -- Vertex shader
    glUniform2f(m.u_position_loc, user_pos.x, user_pos.y);
    glUniform2f(m.u_scale_loc, u_scale.x, u_scale.y);
    glUniform1f(m.u_rotation_loc, u_rotation);

    glUniform2f(m.u_camera_position_loc, camera_pos.x, camera_pos.y);
    glUniform2f(m.u_camera_viewport_size_loc, camera_viewport_size.x,
                camera_viewport_size.y);
    glUniform1f(m.u_camera_pixels_per_unit_loc, camera_pixels_per_unit);

    // -- Fragment shader
    glUniform1f(m.u_time_loc, u_time);
    glUniform1f(m.u_size_ratio_loc, size_ratio);
    glUniform2f(m.u_movement_vector_loc, movement_vector.x, movement_vector.y);
    glUniform1f(m.u_age_loc, age);
}