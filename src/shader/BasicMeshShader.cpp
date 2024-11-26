#include "BasicMeshShader.hpp"

#include "util/gl_compile_program.hpp"

BasicMeshShader::BasicMeshShader() {
    m.program = gl_compile_program(
        // vertex shader
        "#version 330\n"
        "layout(location = 0) in vec2 a_position;\n"
        "layout(location = 1) in vec2 a_texture_coord;\n"
        "layout(location = 2) in vec4 a_color;\n"
        "uniform vec2 u_position;\n"                 // from transform
        "uniform vec2 u_scale;\n"                    // from transform
        "uniform float u_rotation;\n"                // from transform
        "uniform vec2 u_camera_position;\n"          // from camera
        "uniform vec2 u_camera_viewport_size;\n"     // from camera
        "uniform float u_camera_pixels_per_unit;\n"  // from camera
        "out vec2 texture_coord;\n"
        "out vec4 color;\n"
        ""
        "void main() {\n"
        "  mat2 rotation_matrix ="
        "  mat2(cos(u_rotation),-sin(u_rotation),\n"
        "     sin(u_rotation),cos(u_rotation));\n"
        "  vec2 world_position = rotation_matrix * (a_position * u_scale) + "
        "u_position;\n"
        "  vec2 camera_space_position = (world_position - "
        "u_camera_position) * u_camera_pixels_per_unit;\n"
        "  vec2 clip_space_position = (camera_space_position / "
        "u_camera_viewport_size) * 2.0;\n"
        "  gl_Position = vec4(clip_space_position, 0.0, 1.0);\n"
        ""
        "  texture_coord = a_texture_coord;\n"
        "  color = a_color;\n"
        "}\n",
        // ----- Fragment shader -----
        "#version 330\n"
        "in vec2 texture_coord;\n"
        "in vec4 color;\n"
        "uniform sampler2D u_frag_texture;\n"
        "uniform bool u_frag_use_texture;\n"
        "out vec4 FragColor;\n"
        "void main() {\n"
        "   if (u_frag_use_texture) {\n"
        "       FragColor = texture(u_frag_texture, texture_coord);\n"
        "   } else {\n"
        "       FragColor = color;\n"
        "   }\n"
        "}\n");
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
    m.u_frag_texture_loc = glGetUniformLocation(m.program, "u_frag_texture");
    m.u_frag_use_texture_loc =
        glGetUniformLocation(m.program, "u_frag_use_texture");
}

BasicMeshShader::~BasicMeshShader() { glDeleteProgram(m.program); }

BasicMeshShader::BasicMeshShader(BasicMeshShader&& other) noexcept
    : m(other.m) {
    other.m.program = 0;
}

BasicMeshShader& BasicMeshShader::operator=(BasicMeshShader&& other) noexcept {
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

//assumed vertex buffer is updated already
void BasicMeshShader::updateUniforms(glm::vec2 user_pos, glm::vec2 u_scale, float u_rotation, 
                                    glm::vec2 camera_pos, glm::vec2 camera_viewport_size, float camera_pixels_per_unit,
                                    int frag_use_texture = 0, GLuint texture = -1U) {
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
        // -- Fragment shader
        glUniform1i(m.u_frag_use_texture_loc, frag_use_texture);

        // Texture
        if (frag_use_texture) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);
            glUniform1i(m.u_frag_texture_loc, 0);
        }
};
