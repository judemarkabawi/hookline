#version 330
layout(location = 0) in vec2 a_position;
layout(location = 1) in vec2 a_texture_coord;
layout(location = 2) in vec4 a_color;
uniform vec2 u_position;                 // from transform
uniform vec2 u_scale;                    // from transform
uniform float u_rotation;                // from transform
uniform vec2 u_camera_position;          // from camera
uniform vec2 u_camera_viewport_size;     // from camera
uniform float u_camera_pixels_per_unit;  // from camera
out vec2 texture_coord;
out vec4 color;

void main() {
  mat2 rotation_matrix = mat2(cos(u_rotation),-sin(u_rotation), sin(u_rotation),cos(u_rotation));
  vec2 world_position = rotation_matrix * (a_position * u_scale) + u_position;
  vec2 camera_space_position = (world_position - u_camera_position) * u_camera_pixels_per_unit;
  vec2 clip_space_position = (camera_space_position / u_camera_viewport_size) * 2.0;
  gl_Position = vec4(clip_space_position, 0.0, 1.0);

  texture_coord = a_texture_coord;
  color = a_color;
}