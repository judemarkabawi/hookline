#version 330
uniform float u_time;
uniform vec2 u_drawable_size;
out vec4 FragColor;
void main() {
   vec2 norm_coords = gl_FragCoord.xy / u_drawable_size; // Strobe
   float strobe = sin(u_time / 100.0) * 0.25 + 0.5;  // 0 - 1 Color mix
   vec4 cyan = vec4(0.0, 0.7, 1.0, 1.0);
   vec4 magenta = vec4(1.0, 0.0, 0.4, 1.0);
   float mix_ratio = (sin(norm_coords.y + u_time * 0.001) * 0.5 + 0.5 +
                      sin(norm_coords.x + u_time * 0.001) * 0.5 + 0.5) / 2.0;

   vec4 result_color = mix(cyan, magenta, mix_ratio);
   FragColor = result_color;
}