#version 330
in vec2 a_position;
in vec2 a_texture_coord;
in vec4 a_color;
uniform vec2 u_position;                 // from transform
uniform vec2 u_scale;                    // from transform
uniform float u_rotation;                // from transform
uniform vec2 u_camera_position;          // from camera
uniform vec2 u_camera_viewport_size;     // from camera
uniform float u_camera_pixels_per_unit;  // from camera
out vec2 texture_coord;
out vec4 color;
        
void main() {
    mat2 rotation_matrix =
    mat2(cos(u_rotation),-sin(u_rotation),
        sin(u_rotation),cos(u_rotation));
    vec2 world_position = rotation_matrix * (a_position * u_scale) + u_position;
    vec2 camera_space_position = (world_position - u_camera_position) * u_camera_pixels_per_unit;
    vec2 clip_space_position = (camera_space_position / u_camera_viewport_size) * 2.0;
    gl_Position = vec4(clip_space_position, 0.0, 1.0);
    texture_coord = a_texture_coord;
    color = a_color;
}

#version 330
in vec2 texture_coord;
in vec4 color;
uniform vec2 u_player_position; 
uniform sampler2D u_frag_texture;
uniform float u_time; 
uniform bool u_frag_use_texture;
out vec4 FragColor;

vec3 palette(float t) {    
    vec3 a = vec3(0.508, 0.500, 0.508);    
    vec3 b = vec3(-0.482, 0.478, 0.418);   
    vec3 c = vec3(0.558, 1.000, 1.000);    
    vec3 d = vec3(0.138, -0.592, 0.667);   
    return a + b*cos( 6.283185*(c*t+d));   
}  
   
float sdBox( in vec2 p, in vec2 b )    
{  
    vec2 d = abs(p)-b; 
    return length(max(d,0.0)) + min(max(d.x,d.y),0.0); 
}  

void main()    
{  
    // Normalized pixel coordinates (from 0 to 1)  
    vec2 uv = 2.0*texture_coord - 1.0; 
    
    float d = sdBox(uv, vec2(1.0, 1.0));   
    d = -d;    
    
    vec3 normal = vec3(0.0);   
    vec3 lightPos = vec3(0.5, 0.5, 0.5);   
    
    float extent = 0.25;   
    float z = max(extent * d, 0.0);    
    vec3 lightRay = normalize(u_player_position - vec3(gl_FragCoord.xy, z)); 
    
    float top = float(uv.x < uv.y)*float(uv.x > -uv.y);    
    float left = float(uv.x < uv.y)*float(uv.x < -uv.y);   
    float right = float(uv.x > uv.y)*float(uv.x > -uv.y);  
    float bot = float(uv.x > uv.y)*float(uv.x < -uv.y);    
    
    vec3 topNorm = vec3(0.0, 0.5, 0.5);    
    vec3 botNorm = vec3(0.0, -0.5, 0.5);   
    vec3 leftNorm = vec3(-0.5, 0.0, 0.5);  
    vec3 rightNorm = vec3(0.5, 0.0, 0.5);  
    
    vec3 norm = normalize(top*topNorm + bot*botNorm + left*leftNorm + right*rightNorm);    
    
    float lighting = max(0.0, dot(lightRay, norm));    
    lighting *= 0.7;   
    lighting += 0.2;   
    
    float M_PI = 3.14159265358;    
    
    float b = d;   
    //light rings  
    d = (0.4*d - 0.1*(length(uv)/0.5));    
    float angle = atan(uv.y, uv.x) + M_PI; 
    
    float off = (sin(angle*1.0 + u_time)/80.0)*(cos(20.0*angle + 5.0 + 
    5.0*u_time)/2.0); 
    d+= off;   
    
    float off2 = (sin(3.5*angle*1.0 + u_time)/80.0)*(cos(12.0*angle + 5.0 + 5.0*u_time)/2.0);    
    b += off2; 
    b = smoothstep(0.0, 0.2, b);   
    b = 0.1 / b;   
    
    d = sin(20.0*d + 2.0*u_time)/20.0;  
    d = abs(d);    
    d = smoothstep(0.0, 0.03, d);  
    d = 0.125 / d; 
    
    float highlight = d + b;   
    vec3 highlight_col = palette(length(uv))*highlight;    
    
    vec3 baseColor = vec3(0.4, 0.8, 0.9);  
    if (u_frag_use_texture) {  
        baseColor = texture(u_frag_texture, texture_coord);    
    } else {   
        baseColor = color; 
    }  
    
    vec3 col = vec3(lighting)*baseColor + highlight_col*0.8;   
   
    // Output to screen    
    FragColor = vec4(col, 1.0);
}