#version 330 
uniform float u_time; 
uniform vec2 u_drawable_size; 
out vec4 FragColor; 
uniform vec2 u_camera_pos;



uniform sampler2D u_bg_emission;
uniform sampler2D u_bg_color;
uniform sampler2D u_bg_normal;

uniform sampler2D u_mg_emission;
uniform sampler2D u_mg_color;
uniform sampler2D u_mg_normal;

uniform sampler2D u_fg_emission;
uniform sampler2D u_fg_color;
uniform sampler2D u_fg_normal;

uniform samplerCube u_bg_cube;

// cosine based palette, 4 vec3 params
//from https://iquilezles.org/articles/palettes/
//and http://dev.thi.ng/gradients/
vec3 palette(float t, vec3 a, vec3 b, vec3 c, vec3 d )
{
    return a + b*cos( 6.283185*(c*t+d) );
}

vec3 hex_palette(float t) {
    vec3 a = vec3(0.5,0.5,0.5);
    vec3 b = vec3(0.5,0.5,0.5);
    vec3 c = vec3(1.0,1.0,1.0);
    vec3 d = vec3(0.0,0.10,0.20);
    vec3 col = palette(t, a, b, c, d);
    return col;
}

vec3 hex_palette2(float t) {
    vec3 a = vec3(0.748, 0.688, 0.778);
    vec3 b = vec3(0.228, 0.228, -0.282);
    vec3 c = vec3(1.448, 0.468, 0.438);
    vec3 d = vec3(0.518, 0.578, 0.008);
    vec3 col = palette(t, a, b, c, d);
    return vec3(clamp(col.x, 0.0, 1.0), clamp(col.y, 0.0, 1.0), clamp(col.z, 0.0, 1.0));
}

vec3 hex_palette3(float t) {
    vec3 a = vec3(1.0,0.5,0.5);
    vec3 b = vec3(0.5,0.5,0.5);
    vec3 c = vec3(1.0,1.0,1.0);
    vec3 d = vec3(0.0,0.10,0.20);
    vec3 col = palette(t, a, b, c, d);
    return col;
}

vec3 hex_palette4(float t) {
    vec3 a = vec3(0.718, 0.478, 0.628);
    vec3 b = vec3(0.288, 0.528, -0.102);
    vec3 c = vec3(0.558, 0.498, 0.438);
    vec3 d = vec3(0.548, 0.548, 0.008);
    vec3 col = palette(t, a, b, c, d);
    return col;
}

vec3 env_pallette(float t) {
    vec3 a = vec3(0.388, 0.508, 0.338);
    vec3 b = vec3(-0.532, 0.248, 0.308);
    vec3 c = vec3(0.338, 1.178, 0.838);
    vec3 d = vec3(1.538, 0.978, 1.148);
    vec3 col = palette(t, a, b, c, d);
    return vec3(clamp(col.x, 0.0, 1.0), clamp(col.y, 0.0, 1.0), clamp(col.z, 0.0, 1.0));
}


float inBeehive(float u, float v, float sqh, float borderWidth) {
   bool up = abs(v - 0.5) <= ((1.0 - (sqh+borderWidth/2.0)))*(0.5 - abs(u - 0.5)) + (sqh+borderWidth/2.0)/2.0;
   bool lo = abs(v - 0.5) <= ((1.0 - (sqh+borderWidth/2.0)))*(0.5 - abs(u - 0.5)) + (sqh-borderWidth/2.0)/2.0;
   float t = (abs(v - 0.5) - ((1.0-sqh)*(0.5 - abs(u - 0.5)) + sqh/2.0 - borderWidth/2.0));
   t = clamp(t/(borderWidth/2.0), 0.0, 1.0);
   bool h = ((borderWidth/2.0 <= u) && (u <= 1.0-(borderWidth/2.0)));
   float b = max(min((u/(borderWidth/2.0)), 1.0f)*min(float((1.0-u)/(borderWidth/2.0)), 1.0), 0.0);
   //not on border
   return max(t, (1.0-b));
}


float dist(float x1, float y1, float x2, float y2) {
    return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}

float remap(float x, float i_s, float i_e, float o_s, float o_e) {
    return o_s + ((x - i_s)/(i_e - i_s))*(o_e-o_s);
}

vec3 remap(vec3 x, float i_s, float i_e, float o_s, float o_e) {
    return o_s + ((x - i_s)/(i_e - i_s))*(o_e-o_s);
}

vec4 hex_shader(vec2 uv, float iTime, vec3 norm, float scale, int type)
{
    float borderWidth = 0.1;
    float sqh = 0.6;

    // Normalized pixel coordinates (from 0 to 1)
    float u = uv.x;
    float v = uv.y;
    
    
    
    float warp = 0.0f;//0.3*cos(iTime*0.3)*((sin(iTime*10.0)+1.0)/2.0);
    
    float off = sin(3.141592*(uv.x + 1.0)) + 1.0;
    //v += 0.5*off*off*sin(3.141592*(uv.y - 0.5));
    v = min(v/(1.0 - warp*off*0.5), 1000.0);
    u = min(u/(1.0 - warp*off*0.7), 1000.0);
    
    u *= scale;
    v *= scale;
    
    u *= remap(abs(norm.y), 0.0, 1.0, 1.0, 1.0);
    v *= remap(abs(norm.z), 0.0, 1.0, 1.0, 1.0);
    
    float u_tot = u;
    float v_tot = v;
    
    
    u = fract(u + 1.0f);
    v = mod(v, 1.0 + sqh);
    
    float u2 = u + 0.5;
    float v2 = v_tot - (sqh + ((1.0-sqh)/2.0));//, 1.0 + sqh + borderWidth);
    
    float u2_tot = u_tot + 0.5;
    float v2_tot = v2;
    
    u2 = fract(u2);
    v2 = mod(v2, 1.0 + sqh);
    
    // Time varying pixel color
    vec3 border_dist = vec3(inBeehive(u, v, sqh, borderWidth)*inBeehive(u2, v2, sqh, borderWidth));
    float uv_hex_dist = inBeehive(u, v, sqh, 1.0);
    float uv2_hex_dist = inBeehive(u2, v2, sqh, 1.0);
    vec3 hex_dist = vec3(1.0 - clamp((uv_hex_dist*uv2_hex_dist)/(1.0-borderWidth/2.0), 0.0, 1.0));
    vec3 circle_dist = 1.0 - vec3(min(dist(u, v, 0.5, 0.5)*0.5, dist(u2, v2, 0.5, 0.5)*0.5));
    hex_dist = (hex_dist+circle_dist)/2.0;
    hex_dist = remap(hex_dist, 0.1, 1.0, 0.2, 1.5);
    hex_dist = hex_dist*(float(border_dist.x <= 0.0));
    border_dist = remap(border_dist, 0.0, 1.0, 0.2, 1.0);
    
    float sects = 13.0;
    u_tot += floor(sects/2.0);
    v_tot += floor(sects/2.0);
    u2_tot += floor(sects/2.0);
    v2_tot += floor(sects/2.0);
    
    float u_idx = round(u_tot - 0.5);
    float v_idx = round((v_tot-0.5)/(1.0+sqh));
    float u2_idx = round(u2_tot - 0.5);
    float v2_idx = round((v2_tot-0.5)/(1.0+sqh));
    
    //vec3 uv_col = 1.3*vec3(u_idx/10.0, v_idx/10.0, 1.0);
    //vec3 uv2_col = 1.3*vec3(u2_idx/10.0, v2_idx/10.0, 0.0);
    
    
    
    float uv_mask = float(uv_hex_dist < 1.0);
    float uv2_mask = float(uv2_hex_dist < 1.0);
    
    float t = 0.3*iTime;
    
    float uv_idx =  (dist(u_idx/sects, v_idx/sects, 2.*cos(2.*t), 2.*sin(2.*t))*0.5) + t ;
    float uv2_idx = (dist(u2_idx/sects, v2_idx/sects, 2.*cos(2.*t+0.2), 2.*sin(2.*t+0.2))*0.5) + t;
    
    vec3 uv_color = vec3(0.0);
    vec3 uv2_color = vec3(0.0);
    if(type == 0) {
        uv_color = hex_palette(fract(uv_idx));
        uv2_color = hex_palette(fract(uv2_idx));
    } else if (type == 1) {
        uv_color = hex_palette2(fract(uv_idx));
        uv2_color = hex_palette2(fract(uv2_idx));
    } else if(type == 2) {
        uv_color = hex_palette3(fract(uv_idx));
        uv2_color = hex_palette3(fract(uv2_idx));
    } else if(type == 3) {
        uv_color = hex_palette4(fract(uv_idx));
        uv2_color = hex_palette4(fract(uv2_idx));
    }
    
    vec3 hex_col = (uv_mask*uv_color + uv2_mask*uv2_color)*max(hex_dist.x, 0.0);
    hex_col = clamp(hex_col, vec3(0.0), vec3(1.0));
    
    vec3 border_col = vec3(0.8, 0.3, 0.0)*remap(border_dist, 0.0, 0.9, 0.0, 0.7);
    vec3 col = (hex_col + float(border_dist.x>0.0)*border_col);
    
    //col = vec3(uu, v, 1.0);
    // Output to screen
    return vec4(col,1.0);
}

vec4 hex_shader2(vec2 uv, float iTime, float scale)
{
    // Normalized pixel coordinates (from 0 to 1)
    float u = uv.x;
    float v = uv.y;
    
    
    float scale_x = scale;
    float scale_y = scale;
    float sqh = 0.0;
    float borderWidth = 1.0;
    
    v += 0.04*sin(((u+0.02*iTime)*4.0 + 0.5)*3.141592);
    u += 0.02*sin(((v+0.05*iTime)*4.0 + 0.5)*3.141592);
    u *= 1.0 - 0.1*((sin(((u)*2.0 + 0.5)*3.141592)+1.0)/2.0);
    
    
    float warp = 0.0f;
    
    float off = sin(3.141592*(uv.x + 1.0)) + 1.0;

    v = min(v/(1.0 - warp*off*0.5), 1000.0);
    u = min(u/(1.0 - warp*off*0.7), 1000.0);

    
    u *= scale_x;
    v *= scale_y;
   
    
    float u_tot = u;
    float v_tot = v;
   
    u = fract(u + 1.0f);
    v = mod(v, 1.0 + sqh);
    
    float u2 = u + 0.5;
    float v2 = v_tot - (sqh + ((1.0-sqh)/2.0));//, 1.0 + sqh + borderWidth);
    
    float u2_tot = u_tot + 0.5;
    float v2_tot = v2;
    
    u2 = fract(u2);
    v2 = mod(v2, 1.0 + sqh);
    
    // Time varying pixel color
    vec3 border_dist = vec3(inBeehive(u, v, sqh, borderWidth)*inBeehive(u2, v2, sqh, borderWidth));
    float uv_hex_dist = inBeehive(u, v, sqh, 1.0);
    float uv2_hex_dist = inBeehive(u2, v2, sqh, 1.0);
    vec3 hex_dist = vec3(1.0 - clamp((uv_hex_dist*uv2_hex_dist)/(1.0-borderWidth/2.0), 0.0, 1.0));
    vec3 circle_dist = 1.0 - vec3(min(dist(u, v, 0.5, 0.5)*0.5, dist(u2, v2, 0.5, 0.5)*0.5));
    hex_dist = (hex_dist+circle_dist)/2.0;
    //hex_dist = remap(hex_dist, 0.1, 1.0, 0.2, 1.5);
    hex_dist = hex_dist*(float(border_dist.x <= 0.0));
    border_dist = remap(border_dist, 0.0, 1.0, 0.2, 1.0);
    
    float d = remap(hex_dist.x + border_dist.x, 0.5, 1.0, 0.0, 1.0);
    
    
    float b = clamp(pow(d, 3.0), 0.0, 1.0);
    float c = abs(d - (0.8*((sin(iTime)+1.0)/2.0) + 0.4*(cos(u_tot + v_tot - iTime)+1.0)/2.0));
    
    c = 0.1*b + (1.0 - c*6.0);
    c += d*0.25;
    //c = clamp(c, 0.0, 1.0);
    c = 0.4/(1.0-min(c, 0.999));
    c = clamp(c, 0.0, 1.0);
    
    vec3 col = 1.0*(0.4*c+0.6)*hex_palette2(c);
    
    // Output to screen
    return vec4(col,1.0);
}


void render_layer(out vec4 color, out float alpha, in float dist, vec2 uv, in vec2 cameraPos, in float MAX_FOG_DIST, in float iTime, 
                  in sampler2D emissionTex, in sampler2D colorTex, in sampler2D normalTex, in samplerCube u_bg_cube) {
    vec2 uv0 = uv;
    uv *= dist/MAX_FOG_DIST;
    uv += cameraPos*(1.0/dist);
    
    vec2 texR = vec2(16.0/9.0, 1.0);
    vec2 _uv_ = vec2(uv.x / texR.x, uv.y / texR.y);
    _uv_ = vec2(mod(_uv_.x, 1.0), _uv_.y);
    vec4 _mat_ = texture(emissionTex, _uv_); 
    vec3 albedo = texture(colorTex, _uv_).xyz;
    vec4 _norm_and_ambient_ = texture(normalTex, _uv_);
    vec3 mat = _mat_.xyz;
    alpha = _mat_.w;
    float ambient = _norm_and_ambient_.w;
    vec3 norm_col = _norm_and_ambient_.xyz;
    
    
    
    //change normals to -1 to range
    bool is_building_mat = mat.r > 0.5 && mat.g > 0.5 && mat.b > 0.5;
    bool is_void = (mat.r < 0.1) && (mat.g < 0.1) && (mat.b < 0.1);
    alpha = is_void ? 0.0 : alpha;
    vec3 norm_vec = norm_col*2.0 - 1.0;
    norm_vec = norm_vec == vec3(0.0) || is_void ? vec3(0.0) : normalize(norm_vec);
    vec3 view_vec = normalize(vec3(-1.0, uv0 - vec2(0.5, 0.25))).xzy;
    vec3 reflect_vec = reflect(view_vec, norm_vec.xzy);
    
    float rot = 3.141592/1.5;
    vec3 sunDir1 = normalize(vec3(cos(rot + iTime*0.05), sin(rot + iTime*0.05), 1.0));
    vec3 sunDir2 = normalize(vec3(cos(rot + 1.7 + iTime*0.05), sin(rot + 1.7 + iTime*0.05), 1.0));
    
    vec3 lumaCoeff = vec3(0.2126, 0.7152, 0.0722);
    float luma = dot(albedo,lumaCoeff);
    vec3 albedoFlat = !is_void ? clamp(albedo/luma, vec3(0.0), vec3(1.0)) : vec3(0.0);
    float lightness = max(dot(norm_vec, sunDir1), 0.0);// + max(dot(norm_vec, sunDir2), 0.0);
    //lightness *= 0.4;
    lightness += luma*0.6;
    //lightness += ambient;
    //lightness *= 0.5;
    //lightness += 0.5;
    
    
    vec3 col = albedoFlat*lightness;
    
    vec3 emission = vec3(0.0);
    if(is_building_mat) {

    } else if (mat.b > 0.5 && mat.g > 0.5) {
        emission = hex_shader(uv, u_time, norm_vec, dist*2.0, 0).xyz;
    } else if (mat.b > 0.5) {
        emission = hex_shader(uv, u_time, norm_vec, dist*2.0, 3).xyz;
    } else if (mat.g > 0.5) {
        emission = hex_shader2(uv, u_time, dist).xyz;
    } else if (mat.r > 0.5) {
        emission = texture(u_bg_cube, reflect_vec).xyz*0.6;
    } else {
        emission = texture(u_bg_cube, view_vec).xyz;
        //alpha = 0.0;
    }
    
    dist = alpha > 0.0 ? dist : MAX_FOG_DIST;
    float fog_factor = uv0.y*0.6*pow(dist/MAX_FOG_DIST, 1.5);
    vec3 fog = env_pallette(fog_factor)*vec3(1.1);// + vec3(0.1, 0.1, 0.05);
    float fog_amt = 0.9;
    
    col += emission;
    
    col = mix(col, fog, fog_amt*(dist/MAX_FOG_DIST));
    col = mix(col, fog, 0.15 + (dist/MAX_FOG_DIST)*min(2.0*fog_amt*(1.0 - pow(uv.y, 0.2)), 1.0));
    

    // Output to screen
    color = vec4(vec3(col), alpha);
}




void main()
{
    // Normalized pixel coordinates (from 0 to 1)
    float minR = min(u_drawable_size.x, u_drawable_size.y);
    float maxR = max(u_drawable_size.x, u_drawable_size.y);
    float aspect_ratio = maxR / minR;
    
    vec2 uv = vec2(gl_FragCoord.xy)/minR;
    float MAX_FOG_DIST = 170.0;
    
    
    
    vec4 color = vec4(0.0);
    float alpha = 0.0;
    //bg
    render_layer(color, alpha, 120.0, uv, u_camera_pos, MAX_FOG_DIST, u_time, u_bg_emission, u_bg_color, u_bg_normal, u_bg_cube);
    FragColor = vec4(color.xyz, 1.0);
    
    //mg
    render_layer(color, alpha, 80.0, uv + 0.5*u_drawable_size.xy/minR + vec2(0.5, 0.0), u_camera_pos, MAX_FOG_DIST, u_time, u_mg_emission, u_mg_color, u_mg_normal, u_bg_cube);
    if(alpha > 0.0) FragColor = mix(FragColor, vec4(vec3(color.xyz), 1.0), alpha);
    
    //fg
    render_layer(color, alpha, 30.0, uv + 0.5*u_drawable_size.xy/minR, u_camera_pos, MAX_FOG_DIST, u_time, u_fg_emission, u_fg_color, u_fg_normal, u_bg_cube);
    if(alpha > 0.0) FragColor = mix(FragColor, vec4(vec3(color.xyz), 1.0), alpha);
}