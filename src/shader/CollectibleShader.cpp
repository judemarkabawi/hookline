#include "CollectibleShader.hpp"

#include "util/gl_compile_program.hpp"

CollectibleShader::CollectibleShader() {
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
"uniform vec2 u_drawable_size;\n"
"uniform float u_size_ratio;\n"
"out vec4 FragColor; \n"
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
"// The MIT License\n"
"// Copyright © 2019 Inigo Quilez\n"
"// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.\n"
"\n"
"// Distance to a regular pentagon, without trigonometric functions. \n"
"\n"
"// List of some other 2D distances: https://www.shadertoy.com/playlist/MXdSRf\n"
"//\n"
"// and iquilezles.org/articles/distfunctions2d\n"
"\n"
"float sdHexagram( in vec2 p, in float r )\n"
"{\n"
"    const vec4 k = vec4(-0.5,0.86602540378,0.57735026919,1.73205080757);\n"
"    \n"
"    p = abs(p);\n"
"    p -= 2.0*min(dot(k.xy,p),0.0)*k.xy;\n"
"    p -= 2.0*min(dot(k.yx,p),0.0)*k.yx;\n"
"    p -= vec2(clamp(p.x,r*k.z,r*k.w),r);\n"
"    return length(p)*sign(p.y);\n"
"}\n"
"\n"
"\n"
"void main()\n"
"{\n"
"    vec4 glow_col = color;\n"
"    \n"
"    // Normalized pixel coordinates (from 0 to 1)\n"
"    float minR = min(u_drawable_size.x, u_drawable_size.y);\n"
//"    vec2 uvFixed = (texture_coord*(u_drawable_size.xy/minR))*2.0 - u_drawable_size.xy/minR;\n"
"    vec2 uvStretched = texture_coord.xy*2.0 - 1.0;\n"
"    vec2 uvFixed = uvStretched; //(texture_coord*(u_drawable_size.xy/minR))*2.0 - u_drawable_size.xy/minR;\n"
"    \n"
"    float d = length(uvStretched);\n"
"    float d_fixed = length(uvFixed);\n"
"    \n"
"    float haloD = d - u_size_ratio;\n"
"    haloD = abs(haloD)/(1.25*u_size_ratio);\n"
"    haloD = 0.2/haloD;\n"
"\n"
"    //little star effects\n"
"    float starR = u_size_ratio * 1.5;\n"
"    float starD = sdHexagram(uvFixed, starR);\n"
"    starD = abs(starD)/starR;\n"
"    starD = (sin(starD*(1.0/(3.0*starR)) - u_time*1.5) + 1.0)/1.5;\n"
"    starD = 0.3 / (starD+0.3);\n"
"    starD = pow(starD, 0.5);\n"
"    starD *= 0.4;\n"
"    \n"
"    float ITERATIONS = 6.0;\n"
"    for(float i = 0.0; i < ITERATIONS; i++) {\n"
"       float r = u_size_ratio;\n"
"       float s = 0.5;\n"
"       float twoPi = 2.0*3.131592;\n"
"       vec2 uvFixed0 = uvFixed;\n"
"       uvFixed0 *= s;\n"
"       float angle = twoPi*(i/ITERATIONS);\n"
"       uvFixed0 += vec2(cos(angle), sin(angle))*r;\n"
"       \n"
"        float newStarR = starR*s;\n"
"        float newStarD = sdHexagram(uvFixed0, newStarR);\n"
"        newStarD = abs(newStarD)/newStarR;\n"
"        newStarD = (sin(newStarD*(1.0/(3.0*newStarR)) - u_time*1.5 + 1.7) + 1.0)/1.5;\n"
"        newStarD = 0.3 / (newStarD+0.3);\n"
"        newStarD = pow(newStarD, 0.5);\n"
"        //newStarD *= 0.4;\n"
"        starD = max(starD, newStarD);\n"
"    }\n"
"    starD = clamp(starD, 0.0, 1.0);\n"
"    starD *= 0.6*min(exp(-5.0*(d_fixed-0.15)), 1.0);\n"
"    \n"
"    float bloomGlow = (sin(d*(1.0/u_size_ratio) - 2.0*u_time) + 1.0)/2.0;\n"
"    bloomGlow = pow(bloomGlow, 1.0);\n"
"    bloomGlow = 0.5/(bloomGlow+0.6);\n"
"    bloomGlow = pow(bloomGlow, 2.0);\n"
"    bloomGlow = clamp(bloomGlow, 0.0, 1.0);\n"
"    bloomGlow *= 0.3;\n"
"    \n"
"    float lightness = bloomGlow + haloD + starD;\n"
"     \n"
"    vec3 hsv = rgb2hsv(glow_col.xyz);\n"
"    hsv.x += d*0.8 - starD*0.2;\n"
"    hsv.y *= 0.6;\n"
"    hsv.z *= 0.9;\n"
"    vec3 col = hsv2rgb(hsv);\n"
//"    col += 0.1;\n"
//"    col = vec3(pow(col.x, 0.9), pow(col.y, 0.9), pow(col.z, 0.9));\n"
"    \n"
"    \n"
"     \n"
"    col *= lightness;\n"
"    float falloff = (1.0 - pow((d_fixed+d)/2.0, 0.5));"
"    col *= falloff; //falloff towards edge\n"
"    lightness = clamp(lightness, 0.0, 1.0);\n"
" \n"
"    // Output to screen\n"
"    FragColor = vec4(col, 1.0);\n"
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
    m.u_drawable_size_loc = glGetUniformLocation(m.program, "u_drawable_size");
    m.u_size_ratio_loc = glGetUniformLocation(m.program, "u_size_ratio");
}

CollectibleShader::~CollectibleShader() { glDeleteProgram(m.program); }

CollectibleShader::CollectibleShader(CollectibleShader&& other) noexcept
    : m(other.m) {
    other.m.program = 0;
}

CollectibleShader& CollectibleShader::operator=(
    CollectibleShader&& other) noexcept {
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

void CollectibleShader::updateUniforms(glm::vec2 user_pos, glm::vec2 u_scale, float u_rotation, 
                        glm::vec2 camera_pos, glm::vec2 camera_viewport_size, float camera_pixels_per_unit,
                        glm::vec2 drawable_size, float u_time, float size_ratio) {
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
        glUniform2f(m.u_drawable_size_loc,
                    drawable_size.x, drawable_size.y);
        glUniform1f(m.u_time_loc, u_time);
        glUniform1f(m.u_size_ratio_loc, size_ratio);
}