#include "CyberpunkBackgroundShaderNew.hpp"

#include "util/gl_compile_program.hpp"

CyberpunkBackgroundShaderNew::CyberpunkBackgroundShaderNew() {
    program = gl_compile_program(
        // vertex shader
        "#version 330 \n \
        in vec2 a_position; \n \
        void main() { \n \
           gl_Position = vec4(a_position, 0.0, 1.0); \n \
        }\n", 
        // ----- Fragment shader ----- 
        "#version 330 \n \
        uniform float u_time; \n \
        uniform vec2 u_drawable_size; \n \
        out vec4 FragColor; \n \
        // cosine based palette, 4 vec3 params \n \
        //from https://iquilezles.org/articles/palettes/ \n \
        //and http://dev.thi.ng/gradients/ \n \
        vec3 palette(float t, vec3 a, vec3 b, vec3 c, vec3 d ) \n \
        { \n \
            return a + b*cos( 6.283185*(c*t+d) ); \n \
        } \n \
         \n \
        vec3 hex_palette(float t) { \n \
            vec3 a = vec3(0.5,0.5,0.5); \n \
            vec3 b = vec3(0.5,0.5,0.5); \n \
            vec3 c = vec3(1.0,1.0,1.0); \n \
            vec3 d = vec3(0.0,0.10,0.20); \n \
            vec3 col = palette(t, a, b, c, d); \n \
            return col; \n \
        } \n \
         \n \
        float inBeehive(float u, float v, float sqh, float borderWidth) { \n \
           bool up = abs(v - 0.5) <= ((1.0 - (sqh+borderWidth/2.0)))*(0.5 - abs(u - 0.5)) + (sqh+borderWidth/2.0)/2.0; \n \
           bool lo = abs(v - 0.5) <= ((1.0 - (sqh+borderWidth/2.0)))*(0.5 - abs(u - 0.5)) + (sqh-borderWidth/2.0)/2.0; \n \
           float t = (abs(v - 0.5) - ((1.0-sqh)*(0.5 - abs(u - 0.5)) + sqh/2.0 - borderWidth/2.0)); \n \
           t = clamp(t/(borderWidth/2.0), 0.0, 1.0); \n \
           bool h = ((borderWidth/2.0 <= u) && (u <= 1.0-(borderWidth/2.0))); \n \
           float b = max(min((u/(borderWidth/2.0)), 1.0f)*min(float((1.0-u)/(borderWidth/2.0)), 1.0), 0.0); \n \
           //not on border \n \
           return max(t, (1.0-b)); \n \
        } \n \
         \n \
         \n \
        float dist(float x1, float y1, float x2, float y2) { \n \
            return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1)); \n \
        } \n \
         \n \
        float remap(float x, float i_s, float i_e, float o_s, float o_e) { \n \
            return o_s + ((x - i_s)/(i_e - i_s))*(o_e-o_s); \n \
        }\n \
         \n \
        vec3 remap(vec3 x, float i_s, float i_e, float o_s, float o_e) { \n \
            return o_s + ((x - i_s)/(i_e - i_s))*(o_e-o_s);  \n \
        } \n \
        void main() { \n \
        // Normalized pixel coordinates (from 0 to 1) \n \
        vec2 uv = gl_FragCoord.xy / u_drawable_size; \n \
        float len = min(u_drawable_size.x, u_drawable_size.y); \n \
        float ud = gl_FragCoord.x/len; \n \
        float u = ((uv.x - 0.5)*2.0);//*(u_drawable_size.x/len); \n \
        float v = gl_FragCoord.y/min(u_drawable_size.x, u_drawable_size.y); \n \
        v = (v - 0.5); \n \
         \n \
        float scale = 20.0; \n \
        float sqh = 0.6; \n \
         \n \
        float warp = 0.0f;//0.3*cos(u_time*0.3)*((sin(u_time*10.0)+1.0)/2.0); \n \
         \n \
        float off = sin(3.141592*(uv.x + 1.0)) + 1.0; \n \
        //v += 0.5*off*off*sin(3.141592*(uv.y - 0.5)); \n \
        v = min(v/(1.0 - warp*off*0.5), 1000.0); \n \
        u = min(u/(1.0 - warp*off*0.7), 1000.0); \n \
        //fragColor = vec4(u); \n \
        //return ; \n \
        \n \
        u *= scale; \n \
        v *= scale; \n \
         \n \
         \n \
         \n \
        float u_tot = u; \n \
        float v_tot = v; \n \
         \n \
        float borderWidth = 0.1; \n \
        u = fract(u + 1.0f); \n \
        v = mod(v, 1.0 + sqh); \n \
         \n \
        float u2 = u + 0.5; \n \
        float v2 = v_tot - (sqh + ((1.0-sqh)/2.0));//, 1.0 + sqh + borderWidth); \n \
         \n \
        float u2_tot = u_tot + 0.5; \n \
        float v2_tot = v2; \n \
         \n \
        u2 = fract(u2); \n \
        v2 = mod(v2, 1.0 + sqh); \n \
         \n \
        // Time varying pixel color \n \
        vec3 border_dist = vec3(inBeehive(u, v, sqh, borderWidth)*inBeehive(u2, v2, sqh, borderWidth)); \n \
        float uv_hex_dist = inBeehive(u, v, sqh, 1.0); \n \
        float uv2_hex_dist = inBeehive(u2, v2, sqh, 1.0); \n \
        vec3 hex_dist = vec3(1.0 - clamp((uv_hex_dist*uv2_hex_dist)/(1.0-borderWidth/2.0), 0.0, 1.0)); \n \
        vec3 circle_dist = 1.0 - vec3(min(dist(u, v, 0.5, 0.5)*0.5, dist(u2, v2, 0.5, 0.5)*0.5)); \n \
        hex_dist = (hex_dist+circle_dist)/2.0; \n \
        hex_dist = remap(hex_dist, 0.1, 1.0, 0.2, 1.5); \n \
        hex_dist = hex_dist*(float(border_dist.x <= 0.0)); \n \
        border_dist = remap(border_dist, 0.0, 1.0, 0.2, 1.0); \n \
         \n \
        float sects = 13.0; \n \
        u_tot += floor(sects/2.0); \n \
        v_tot += floor(sects/2.0); \n \
        u2_tot += floor(sects/2.0); \n \
        v2_tot += floor(sects/2.0); \n \
         \n \
        float u_idx = round(u_tot - 0.5); \n \
        float v_idx = round((v_tot-0.5)/(1.0+sqh)); \n \
        float u2_idx = round(u2_tot - 0.5); \n \
        float v2_idx = round((v2_tot-0.5)/(1.0+sqh)); \n \
         \n \
        //vec3 uv_col = 1.3*vec3(u_idx/10.0, v_idx/10.0, 1.0); \n \
        //vec3 uv2_col = 1.3*vec3(u2_idx/10.0, v2_idx/10.0, 0.0); \n \
         \n \
         \n \
         \n \
        float uv_mask = float(uv_hex_dist < 1.0); \n \
        float uv2_mask   = float(uv2_hex_dist < 1.0); \n \
         \n \
        float t = 0.3*u_time; \n \
         \n \
        float uv_idx =  (dist(u_idx/sects, v_idx/sects, 2.*cos(2.*t), 2.*sin(2.*t))*0.5) + t ; \n \
        float uv2_idx = (dist(u2_idx/sects, v2_idx/sects, 2.*cos(2.*t+0.2), 2.*sin(2.*t+0.2))*0.5) + t; \n \
         \n \
        vec3 uv_color = hex_palette(fract(uv_idx)); \n \
        vec3 uv2_color = hex_palette(fract(uv2_idx)); \n \
         \n \
        vec3 hex_col = (uv_mask*uv_color + uv2_mask*uv2_color)*max(hex_dist.x, 0.0); \n \
        hex_col = clamp(hex_col, vec3(0.0), vec3(1.0)); \n \
         \n \
        vec3 border_col = vec3(0.8, 0.3, 0.0)*remap(border_dist, 0.0, 0.9, 0.0, 0.7); \n \
        vec3 col = /*vec3(0.4, 0.4, 0.6)*/(hex_col + float(border_dist.x>0.0)*border_col); \n \
        FragColor = vec4(col, 1.0); \n \
        }\n");

    a_position_loc = glGetAttribLocation(program, "a_position");
    u_time_loc = glGetUniformLocation(program, "u_time");
    u_drawable_size_loc = glGetUniformLocation(program, "u_drawable_size");
}

CyberpunkBackgroundShaderNew::~CyberpunkBackgroundShaderNew() {
    glDeleteProgram(program);
}

CyberpunkBackgroundShaderNew::CyberpunkBackgroundShaderNew(
    CyberpunkBackgroundShaderNew&& other) noexcept
    : program(other.program),
      a_position_loc(other.a_position_loc),
      u_time_loc(other.u_time_loc),
      u_drawable_size_loc(other.u_drawable_size_loc) {
    other.program = 0;
}

CyberpunkBackgroundShaderNew& CyberpunkBackgroundShaderNew::operator=(
    CyberpunkBackgroundShaderNew&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    // Delete current program
    glDeleteProgram(program);

    // Move
    program = other.program;
    a_position_loc = other.a_position_loc;
    u_time_loc = other.u_time_loc;
    u_drawable_size_loc = other.u_drawable_size_loc;

    // Reset other
    other.program = 0;

    return *this;
}