#version 330
in vec2 texture_coord;
uniform sampler2D u_frag_texture;
out vec4 FragColor;

void main() {
    FragColor = vec4(1.0, 1.0, 1.0, texture(u_frag_texture, texture_coord).r);
}