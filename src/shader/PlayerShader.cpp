#include "PlayerShader.hpp"

#include "util/gl_compile_program.hpp"

PlayerShader::PlayerShader() {
    m.program = gl_compile_program(
"#version 330\n"
"in vec2 a_position;\n"
"in vec2 a_texture_coord;\n"
"in vec4 a_color;\n"
"uniform vec2 u_position;                 // from transform\n"
"uniform vec2 u_scale;                    // from transform\n"
"uniform float u_rotation;                // from transform\n"
"uniform vec2 u_camera_position;          // from camera\n"
"uniform vec2 u_camera_viewport_size;     // from camera\n"
"uniform float u_camera_pixels_per_unit;  // from camera\n"
"out vec2 texture_coord;\n"
"out vec4 color;\n"
"        \n"
"void main() {\n"
"    mat2 rotation_matrix =\n"
"    mat2(cos(u_rotation),-sin(u_rotation),\n"
"        sin(u_rotation),cos(u_rotation));\n"
"    vec2 world_position = rotation_matrix * (a_position * u_scale) + u_position;\n"
"    vec2 camera_space_position = (world_position - u_camera_position) * u_camera_pixels_per_unit;\n"
"    vec2 clip_space_position = (camera_space_position / u_camera_viewport_size) * 2.0;\n"
"    gl_Position = vec4(clip_space_position, 0.0, 1.0);\n"
"    texture_coord = a_texture_coord;\n"
"    color = a_color;\n"
"}\n"
"\n",
"#version 330\n"
"in vec2 texture_coord;\n"
"in vec4 color;\n"
"uniform float u_time; \n"
"uniform float u_health; \n"
"out vec4 FragColor;\n"
"float atan2(float y, float x) { return mod(atan(y,x) + 3.141592, 2.0*3.141592); }\n"
"void main()    \n"
"{  \n"
"    // Output to screen    \n"
"    vec2 uv = texture_coord*2.0 - 1.0;\n"
"    float angle = atan2(uv.y, uv.x);\n"
"    angle += 3.141592;\n"
"    angle /= 2.0*3.141592;\n"
"    FragColor = vec4(vec3(float(angle <= u_health)), 1.0);\n"
"}\n");

    m.a_position_loc = glGetAttribLocation(m.program, "a_position");
    m.a_texture_coord_loc = glGetAttribLocation(m.program, "a_texture_coord");
    m.a_color_loc = glGetAttribLocation(m.program, "a_color");

    m.u_position_loc = glGetUniformLocation(m.program, "u_position");
    m.u_scale_loc = glGetUniformLocation(m.program, "u_scale");
    m.u_rotation_loc = glGetUniformLocation(m.program, "u_rotation");
    
    m.u_camera_position_loc = glGetUniformLocation(m.program, "u_camera_position");
    m.u_camera_viewport_size_loc = glGetUniformLocation(m.program, "u_camera_viewport_size");
    m.u_camera_pixels_per_unit_loc = glGetUniformLocation(m.program, "u_camera_pixels_per_unit");

    m.u_time_loc = glGetUniformLocation(m.program, "u_time");
    m.u_health_loc = glGetUniformLocation(m.program, "u_health");
}

PlayerShader::~PlayerShader() { glDeleteProgram(m.program); }

PlayerShader::PlayerShader(PlayerShader&& other) noexcept
    : m(other.m) {
    other.m.program = 0;
}

PlayerShader& PlayerShader::operator=(
    PlayerShader&& other) noexcept {
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

void PlayerShader::updateUniforms(glm::vec2 user_pos, glm::vec2 u_scale, float u_rotation, 
                        glm::vec2 camera_pos, glm::vec2 camera_viewport_size, float camera_pixels_per_unit,
                        float time, float health) {
        // Uniforms
        // -- Vertex shader
        glUniform2f(m.u_position_loc, user_pos.x,
                    user_pos.y);
        glUniform2f(m.u_scale_loc, u_scale.x,
                    u_scale.y);
        glUniform1f(m.u_rotation_loc, u_rotation);
        glUniform2f(m.u_camera_position_loc,
                    camera_pos.x, camera_pos.y);
        glUniform2f(m.u_camera_viewport_size_loc,
                    camera_viewport_size.x, camera_viewport_size.y);
        glUniform1f(m.u_camera_pixels_per_unit_loc,
                    camera_pixels_per_unit);

        glUniform1f(m.u_time_loc, time);
        glUniform1f(m.u_health_loc, health);
}