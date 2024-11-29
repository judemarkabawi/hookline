#version 330
in vec2 texture_coord;
in vec4 color;
uniform sampler2D u_frag_texture;
uniform bool u_frag_use_texture;
out vec4 FragColor;

void main() {
    if (u_frag_use_texture) {
        FragColor = texture(u_frag_texture, texture_coord);
    } else {
        FragColor = color;
    }
}