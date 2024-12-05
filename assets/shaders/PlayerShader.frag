#version 330
in vec2 texture_coord;
in vec4 color;
uniform float u_time; 
uniform float u_health; 
out vec4 FragColor;

float sqr_dist(float x, float y) {
   return clamp(-(abs(x*x*x*x*x) + abs(y*y*y*y*y) - 1.0), 0.0, 1.0);
}

const float PI = 3.141592;
const float DIVET_HEIGHT = 0.5;
const float INSET_WIDTH = 0.7;
const float INSET_BEVEL_LENGTH = 0.25;
const float DIAGONAL_THICKNESS = 0.075;
const float DIAG_BEV_THICKNESS = 0.1;

//returns vec3(height, divet_mask, glass height)
vec3 height(float x, float y) {
    float inset = clamp(length(vec2(x, y))/INSET_WIDTH, 0.0, 1.0);
    //inset = 1.0 - inset;
    inset = smoothstep((1.0 - INSET_BEVEL_LENGTH), 1.0, inset);
    //inset = 1.0 - inset;
    
    float diagonal_thickness = 0.075;
    float div_thickness = 0.1;
    float r = smoothstep(0.0, DIAG_BEV_THICKNESS, abs(float(x - y)) - DIAGONAL_THICKNESS);
    float l = smoothstep(0.0, DIAG_BEV_THICKNESS, abs(float(y + x)) - DIAGONAL_THICKNESS);
    
    float diag = l*r*inset;
    diag = (1.0 - diag)*(1.0 - DIVET_HEIGHT);
    
    float shooter = clamp(cos(clamp(x, -1.0, 1.0)*PI), 0.0, 1.0);
    
    float sqrd = sqr_dist(x, y);
    
    float ret = clamp(sqrd - diag, 0.0, 1.0);
    
    float glass_r = clamp(abs(float(x - y))/DIAGONAL_THICKNESS, 0.0, 1.0);
    glass_r = cos(glass_r*PI/2.0)*0.6;
    float glass_l = clamp(abs(float(y + x))/DIAGONAL_THICKNESS, 0.0, 1.0);
    glass_l = cos(glass_l*PI/2.0)*0.6;
    float glass_circ = clamp(length(vec2(x, y))/(INSET_WIDTH*(1.0-INSET_BEVEL_LENGTH)), 0.0, 1.0);
    glass_circ = cos(glass_circ*PI/2.0)*1.5;
    float glass_h = max(max(glass_r,  glass_l), glass_circ);
    
    float base = ret + shooter*float(ret <= 0.0);
    base += 0.5*glass_h*float(ret > 0.0);
    return vec3(base, 
                float(diag >= (1.0-DIVET_HEIGHT) && ret > 0.0),
                glass_h*float(ret > 0.0));
}

// cosine based palette, 4 vec3 params
vec3 palette( in float t)
{
    vec3 a = vec3(0.8, 0.5, 0.4);
    vec3 b = vec3(0.2, 0.4, 0.2);
    vec3 c = vec3(2.0, 1.0, 1.0);
    vec3 d = vec3(0.00, 0.25, 0.25);
    return a + b*cos( 6.283185*(c*t+d) );
}

float remap(float x, float i_s, float i_e, float o_s, float o_e) {
    return o_s + ((x - i_s)/(i_e - i_s))*(o_e-o_s);
}

void main()
{
    // Normalized pixel coordinates (from 0 to 1)
    float y_off = 0.025;
    vec2 uv = texture_coord*(2.0+y_off) - 1.0 - 0.5*y_off;
    uv.y += 0.5*y_off;
    
    float angle = atan(uv.y, uv.x)/(2.0*PI);
    angle += float(angle < 0.0);

    // Time varying pixel color
    float health = 1.0;
    float rot = float(angle < health);
    
    
    float x = uv.x;
    float y = uv.y;
    //circle
    float squircle = float(abs(x*x*x*x*x) + abs(y*y*y*y*y) < 1.0);
   
    
    vec3 _height_ret_ = height(x, y);
    
    float dist = _height_ret_.x;
    float x_diff = dist - height(x+0.001, y).x;
    float y_diff = dist - height(x, y+0.001).x;
    x_diff = remap(x_diff, -0.005, 0.005, -1.0, 1.0);
    y_diff = remap(y_diff, -0.005, 0.005, -1.0, 1.0);
    vec3 norm = normalize(vec3(vec2(x_diff, y_diff), 1.0-sqrt(x_diff*x_diff+y_diff*y_diff)));
    
    vec3 sun_dir = normalize(vec3(-1.0, -1.0, 0.0));
    vec3 sun2_dir = normalize(vec3(0.1, 0.3, -1.0));
    float sun1_w = 0.8;
    vec3 dir = sun1_w*clamp(vec3(dot(sun_dir, -norm)), 0.0, 1.0);
    dir += clamp((1.0-sun1_w)*dot(sun2_dir, -norm), 0.0, 1.0);
    dir *= 1.5;
    
    float diag_mask = _height_ret_.y;
    const float PIX_H = 0.2;
    const float PIX_B = 0.25;
    float pix_w = length(uv)/PIX_H;
    float off_w = pix_w - float(int(pix_w));
    pix_w = (pix_w - off_w)*PIX_H;
    float w_mask = float(abs(off_w - 0.5)*2.0 < (1.0 - PIX_B));
    //health decline
    w_mask *= rot;
    
    vec3 glow = w_mask*palette(0.5*pix_w - 0.3*u_time)*diag_mask;
    
    vec3 col = glow + (1.0 - diag_mask*w_mask)*dir*(vec3(0.2, 0.2, 0.4)+0.3);
    col *= float(dist > 0.0);
    //glass specular
    vec3 spec_dir = normalize(vec3(-0.45, -0.6, -1.0));
    float spec = clamp(dot(spec_dir, -norm), 0.0, 1.0);
    spec = smoothstep(0.98, 0.99, spec);
    col += float(_height_ret_.z > 0.001)*spec;
    
    vec3 spec_dir_2 = normalize(vec3(0.0, -0.8, -1.0));
    float spec_2 = clamp(dot(spec_dir_2, -norm), 0.0, 1.0);
    spec_2 = smoothstep(0.995, 0.999, spec_2);
    col += float(_height_ret_.z > 0.001)*spec_2;
    
    float circle_radius = INSET_WIDTH*(1.0-INSET_BEVEL_LENGTH);
    float circ_x = (cos((uv.x-circle_radius)*(PI/2.0)/circle_radius)+1.0)/2.0;
    float circ_y = (cos((uv.y-circle_radius)*(PI/2.0)/circle_radius)+1.0)/2.0;
    vec3 circle_norm = vec3(circ_x, circ_y, 1.0-sqrt(circ_x*circ_x+circ_y*circ_y));
    
    // Output to screen
    FragColor = vec4(vec3(col), float(dist > 0.0));
}