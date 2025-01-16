#version 330
uniform float u_time;
uniform vec2 u_drawable_size;
out vec4 FragColor;
void main() {
   vec2 norm_coords = gl_FragCoord.xy / u_drawable_size; // Strobe
   float strobe = sin(u_time / 100.0) * 0.25 + 0.5;  // 0 - 1 Color mix
   vec4 color1 = vec4(0.77, 0.48, 0.1, 1.0);
   vec4 color2 = vec4(0.83, 0.58, 0.2, 1.0);
   float mix_ratio = (sin(norm_coords.y + u_time * 0.001) * 0.5 + 0.5 +
      sin(norm_coords.x + u_time * 0.001) * 0.5 + 0.5) / 2.0;

   vec4 result_color = mix(color1, color2, mix_ratio);
   result_color += 0.1 * result_color * (sin(norm_coords.y * 6.28) + sin(norm_coords.x * 6.28)) * sin(norm_coords.x * 6.28);
   FragColor = result_color;
}