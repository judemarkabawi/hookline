#include "ProjectileShader.hpp"

#include "util/gl_compile_program.hpp"

ProjectileShader::ProjectileShader() {
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
"\n"
"#version 330\n"
"in vec2 texture_coord;\n"
"in vec4 color;\n"
"uniform float u_time;\n"
"uniform float u_size_ratio;\n"
"uniform vec2 u_movement_vector;"
"uniform float u_age; \n"
"out vec4 FragColor; \n"
"\n"
"\n"
"//conversion functions from\n"
"//https://gist.github.com/983/e170a24ae8eba2cd174f\n"
"vec3 rgb2hsv(vec3 c)\n"
"{\n"
"    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);\n"
"    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));\n"
"    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));\n"
"\n"
"    float d = q.x - min(q.w, q.y);\n"
"    float e = 1.0e-10;\n"
"    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);\n"
"}\n"
"\n"
"vec3 hsv2rgb(vec3 c)\n"
"{\n"
"    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);\n"
"    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);\n"
"    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);\n"
"}\n"
"\n"
"float sdParabolaSeg( in vec2 pos, in float wi, in float he )\n"
"{\n"
"    pos.x = abs(pos.x);\n"
"    float ik = wi*wi/he;\n"
"    float p = ik*(he-pos.y-0.5*ik)/3.0;\n"
"    float q = pos.x*ik*ik*0.25;\n"
"    float h = q*q - p*p*p;\n"
"    float r = sqrt(abs(h));\n"
"    float x = (h>0.0) ? \n"
"        pow(q+r,1.0/3.0) - pow(abs(q-r),1.0/3.0)*sign(r-q) :\n"
"        2.0*cos(atan(r/q)/3.0)*sqrt(p);\n"
"    x = min(x,wi);\n"
"    return length(pos-vec2(x,he-x*x/ik)) * \n"
"           sign(ik*(pos.y-he)+pos.x*pos.x);\n"
"}\n"
"\n"
"float sdParabola( in vec2 pos, in float k )\n"
"{\n"
"    pos.x = abs(pos.x);\n"
"    float ik = 1.0/k;\n"
"    float p = ik*(pos.y - 0.5*ik)/3.0;\n"
"    float q = 0.25*ik*ik*pos.x;\n"
"    float h = q*q - p*p*p;\n"
"    float r = sqrt(abs(h));\n"
"    float x = (h>0.0) ? \n"
"        pow(q+r,1.0/3.0) - pow(abs(q-r),1.0/3.0)*sign(r-q) :\n"
"        2.0*cos(atan(r,q)/3.0)*sqrt(p);\n"
"    return length(pos-vec2(x,k*x*x)) * sign(pos.x-x);\n"
"}\n"
"\n"
"float atan2(float y, float x) { return mod(atan(y,x) + 3.141592, 2.0*3.141592); }\n"
"\n"
"\n"
"void main()\n"
"{    \n"
"    vec2 uv = texture_coord*2.0 - 1.0;\n"
"    \n"
"    \n"
"    float dist = length(uv);\n"
"    float angle = atan2(-u_movement_vector.x, -u_movement_vector.y);\n"
"    \n"
"    float PI = 3.141592;\n"
"    vec2 oldUV = uv;\n"
"    uv.x = cos(angle)*oldUV.x - sin(angle)*oldUV.y;\n"
"    uv.y = sin(angle)*oldUV.x + cos(angle)*oldUV.y;\n"
"   \n"
"    \n"
"    \n"
"    vec2 uv0 = uv;\n"
"    float size_inv = 1.0/u_size_ratio;\n"
"    uv *= size_inv;\n"
"    uv.y -=  u_size_ratio;\n"
"    float wave = (sin(uv.y*5.0 + 20.0*u_time - 10.0*cos(uv.x))+1.0)*0.3;\n"
"\n"
"    float d = sdParabola(uv, -u_size_ratio*3.0)/(u_size_ratio);\n"
"    wave *= (0.5 - d);\n"
"    wave = 0.05 / wave;\n"
"    float p = d;\n"
"    p = abs(p);\n"
"    p = exp(-u_size_ratio*(p-0.3)*2.0);\n"
"    \n"
"    float falloff = 2.0*u_size_ratio - pow(length(uv0 ), 0.8);\n"
"    p *= falloff;\n"
"    p += max(-d*0.7, -0.4)*p;\n"
"    \n"
"    p += 0.0075/length(oldUV)/u_size_ratio;\n"
"   \n"
"    \n"
"     wave *= p;\n"
"     p += wave;\n"
"     \n"
"     float falloff2 = max(1.0 - min(pow(length(oldUV), 2.0), 1.0), 0.0);\n"
"     p *= falloff2;\n"
"     \n"
"     vec3 hsv = rgb2hsv(color.xyz);\n"
"     hsv.x += -0.1 + dist*u_size_ratio*0.4;\n"
"     hsv.y *= 0.8;\n"
"     vec3 col = hsv2rgb(hsv);\n"
"     col += 0.3;\n"
"     \n"
"     \n"
"     col *= p;\n"
"    col *= 1.0 - smoothstep(0.95, 1.0, u_age);"
"    // Output to screen\n"
"    FragColor = vec4(col, 1.0);\n"
"}\n"
"\n");

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
    m.u_movement_vector_loc = glGetUniformLocation(m.program, "u_movement_vector");
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

void ProjectileShader::updateUniforms(glm::vec2 user_pos, glm::vec2 u_scale, float u_rotation, 
                        glm::vec2 camera_pos, glm::vec2 camera_viewport_size, float camera_pixels_per_unit,
                        float u_time, glm::vec2 movement_vector, float size_ratio, float age) {
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
        glUniform1f(m.u_time_loc, u_time);
        glUniform1f(m.u_size_ratio_loc, size_ratio);
        glUniform2f(m.u_movement_vector_loc,
                    movement_vector.x, movement_vector.y);
         glUniform1f(m.u_age_loc, age);
}