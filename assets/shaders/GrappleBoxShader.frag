#version 330
in vec2 texture_coord;
in vec4 color;
in vec2 s_position;
uniform vec2 u_resolution;
uniform vec2 u_scale;
uniform sampler2D u_frag_texture;
uniform float u_time; 
uniform bool u_frag_use_texture;

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
    // Normalized pixel coordinates (from 0 to 1)  
    vec2 uv = 2.0*texture_coord - 1.0; 
    
    float scale_min = min(u_scale.x, u_scale.y);
    vec2 uvs = 2.0*texture_coord*u_scale/scale_min - u_scale/scale_min;

    float d = sdBox(uvs, vec2(1.0, 1.0));   
    d = -d;    
    
    vec3 normal = vec3(0.0);   
    vec3 lightPos = vec3(0.5, 0.5, 0.5);   
    
    float extent = 0.25;   
    float z = max(extent * d, 0.0);

    float res_min = min(u_resolution.x, u_resolution.y);
    vec2 sc_pos = gl_FragCoord.xy/res_min*2.0 - u_resolution/res_min;
    vec3 lightRay = normalize(vec3(sc_pos, z) - vec3(0.0 , 0.0, 1.0)); 
    
    float top = float(uv.x < uv.y)*float(uv.x > -uv.y);    
    float left = float(uv.x < uv.y)*float(uv.x < -uv.y);   
    float right = float(uv.x > uv.y)*float(uv.x > -uv.y);  
    float bot = float(uv.x > uv.y)*float(uv.x < -uv.y);    
    
    vec3 topNorm = vec3(0.0, 0.5, 0.5);    
    vec3 botNorm = vec3(0.0, -0.5, 0.5);   
    vec3 leftNorm = vec3(-0.5, 0.0, 0.5);  
    vec3 rightNorm = vec3(0.5, 0.0, 0.5);  
    
    vec3 norm = normalize(top*topNorm + bot*botNorm + left*leftNorm + right*rightNorm);    
    
    float lighting = max(0.0, dot(lightRay, -norm));    
    lighting *= 0.95;   
    lighting += 0.2;   
    
    float M_PI = 3.14159265358;    
    
    float b = -sdBox(uv, vec2(1.0, 1.0));    
    //light rings  
    d = (0.4*d - 0.1*(length(uvs)/0.5));    
    float angle = atan(uvs.y, uvs.x) + M_PI; 
    
    float off = (sin(angle*1.0 + u_time)/80.0)*(cos(20.0*angle + 5.0 + 5.0*u_time)/2.0); 
    d+= off;   
    
    float off2 = (sin(3.5*angle*1.0 + u_time)/80.0)*(cos(12.0*angle + 5.0 + 5.0*u_time)/2.0);    
    b += off2; 
    b = smoothstep(0.0, 0.2, b);   
    b = 0.1 / b;   
    
    d = sin(2.0*d + 4.0*u_time)/50.0;
    d = abs(d);    
    d = smoothstep(0.0, 0.03, d);  
    d = 0.125 / d; 
    
    float highlight = (d + b) - 0.3;   
    vec3 highlight_col = palette(length(uv))*highlight;    
    
    vec3 baseColor = vec3(0.4, 0.8, 0.9);  
    float alpha = 1.0;
    if (u_frag_use_texture) {  
        vec4 tex_color = texture(u_frag_texture, texture_coord);
        baseColor = tex_color.xyz;    
        alpha = tex_color.w;
    } else {   
        baseColor = color.xyz; 
    }  
    
    vec3 col = vec3(lighting)*baseColor + highlight_col*0.8;   
   
    // Output to screen    
    FragColor = vec4(vec3(col), 1.0);
}