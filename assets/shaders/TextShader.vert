#version 330
layout(location = 0) in vec2 a_position;
layout(location = 1) in vec2 a_texture_coord;
layout(location = 2) in vec4 a_color;
uniform vec2 u_position;  // from transform
out vec2 texture_coord;

void main() {
    vec2 world_position = a_position + u_position;
    gl_Position = vec4(world_position, 0.0, 1.0);

    texture_coord = a_texture_coord;
}
