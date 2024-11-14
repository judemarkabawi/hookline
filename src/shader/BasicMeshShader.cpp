#include "BasicMeshShader.hpp"

#include "util/gl_compile_program.hpp"

BasicMeshShader::BasicMeshShader() {
    program = gl_compile_program(
        // vertex shader
        "#version 330\n"
        "in vec2 a_position;\n"
        "in vec2 a_texture_coord;\n"
        "in vec4 a_color;\n"
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

    a_position_loc = glGetAttribLocation(program, "a_position");
    a_texture_coord_loc = glGetAttribLocation(program, "a_texture_coord");
    a_color_loc = glGetAttribLocation(program, "a_color");
    u_position_loc = glGetUniformLocation(program, "u_position");
    u_scale_loc = glGetUniformLocation(program, "u_scale");
    u_rotation_loc = glGetUniformLocation(program, "u_rotation");
    u_camera_position_loc = glGetUniformLocation(program, "u_camera_position");
    u_camera_viewport_size_loc =
        glGetUniformLocation(program, "u_camera_viewport_size");
    u_camera_pixels_per_unit_loc =
        glGetUniformLocation(program, "u_camera_pixels_per_unit");
    u_frag_texture_loc = glGetUniformLocation(program, "u_frag_texture");
    u_frag_use_texture_loc =
        glGetUniformLocation(program, "u_frag_use_texture");
}

BasicMeshShader::~BasicMeshShader() { glDeleteProgram(program); }

BasicMeshShader::BasicMeshShader(BasicMeshShader&& other) noexcept
    : program(other.program),
      a_position_loc(other.a_position_loc),
      a_texture_coord_loc(other.a_texture_coord_loc),
      a_color_loc(other.a_color_loc),
      u_position_loc(other.u_position_loc),
      u_scale_loc(other.u_scale_loc),
      u_rotation_loc(other.u_rotation_loc),
      u_camera_position_loc(other.u_camera_position_loc),
      u_camera_viewport_size_loc(other.u_camera_viewport_size_loc),
      u_camera_pixels_per_unit_loc(other.u_camera_pixels_per_unit_loc),
      u_frag_texture_loc(other.u_frag_texture_loc),
      u_frag_use_texture_loc(other.u_frag_use_texture_loc) {
    other.program = 0;
}

BasicMeshShader& BasicMeshShader::operator=(BasicMeshShader&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    // Delete current program
    glDeleteProgram(program);

    // Move from other
    program = other.program;
    a_position_loc = other.a_position_loc;
    a_texture_coord_loc = other.a_texture_coord_loc;
    a_color_loc = other.a_color_loc;
    u_position_loc = other.u_position_loc;
    u_scale_loc = other.u_scale_loc;
    u_rotation_loc = other.u_rotation_loc;
    u_camera_position_loc = other.u_camera_position_loc;
    u_camera_viewport_size_loc = other.u_camera_viewport_size_loc;
    u_camera_pixels_per_unit_loc = other.u_camera_pixels_per_unit_loc;
    u_frag_texture_loc = other.u_frag_texture_loc;
    u_frag_use_texture_loc = other.u_frag_use_texture_loc;

    // Reset other
    other.program = 0;

    return *this;
}
