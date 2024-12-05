#version 330
in vec2 texture_coord;
in vec4 color;
uniform vec2 u_resolution;
uniform vec2 u_scale;
uniform float u_time; 

out vec4 FragColor;

vec3 palette(float t) {    
    vec3 a = vec3(0.5, 0.5, 0.5);    
    vec3 b = vec3(0.5, 0.5, 0.5);   
    vec3 c = vec3(2.0, 1.0, 0.0);    
    vec3 d = vec3(0.50, 0.20, 0.25);   
    return a + b*cos( 6.283185*(c*t+d));   
}  
   
float sdBox( in vec2 p, in vec2 b )    
{  
    vec2 d = abs(p)-b; 
    return length(max(d,0.0)) + min(max(d.x,d.y),0.0); 
}  

void main()    
{  
    const float UNIFORM_SCALE = 10.0;
    vec2 uv = texture_coord;
    float scale_min = min(u_scale.x, u_scale.y);
    uv *= u_scale/scale_min;

    float l = min(cos(uv.x*2.0) + 0.9, 1.0);

    float glow_mask = float(l < 0.0);
    vec3 col = glow_mask*palette(uv.x/10.0 - u_time)*0.6 + (1.0 - glow_mask)*l*vec3(0.35, 0.35, 0.15)*(0.3+0.7*abs(uv.y*2.0-0.5));

    FragColor = vec4(vec3(col), 1.0);
}