#version 330
in vec2 texture_coord;
in vec4 color;
uniform vec2 u_scale;
uniform vec2 u_resolution;
uniform sampler2D u_frag_texture;
uniform bool u_frag_use_texture;

out vec4 FragColor;
   
float sdBox( in vec2 p, in vec2 b )    
{  
    vec2 d = abs(p)-b; 
    return length(max(d,0.0)) + min(max(d.x,d.y),0.0); 
}  


void main()    
{  

    const float UNIFORM_SCALE = 30.0f;
    // Normalized pixel coordinates (from 0 to 1)     
    float scale_min = min(u_scale.x, u_scale.y);
    vec2 uv = texture_coord*u_scale*UNIFORM_SCALE;
    uv = vec2(fract(uv.x), fract(uv.y));
    uv = uv*2.0 - 1.0;

    
    float top = float(uv.x < uv.y)*float(uv.x > -uv.y);    
    float left = float(uv.x < uv.y)*float(uv.x < -uv.y);   
    float right = float(uv.x > uv.y)*float(uv.x > -uv.y);  
    float bot = float(uv.x > uv.y)*float(uv.x < -uv.y);    
    
    vec3 topNorm = vec3(0.0, 0.5, 0.5);    
    vec3 botNorm = vec3(0.0, -0.5, 0.5);   
    vec3 leftNorm = vec3(-0.5, 0.0, 0.5);  
    vec3 rightNorm = vec3(0.5, 0.0, 0.5);  
    
    vec3 norm = normalize(top*topNorm + bot*botNorm + left*leftNorm + right*rightNorm);    
    
    float d = sdBox(uv, vec2(1.0, 1.0));   
    d = -d; 
    
    float extent = 0.25; 
    float z = max(extent * d, 0.0);

    float res_min = min(u_resolution.x, u_resolution.y);
    vec2 sc_pos = gl_FragCoord.xy/res_min*2.0 - vec2(u_resolution.x/res_min, 0.2*u_resolution.y/res_min);
    vec3 lightRay = normalize(vec3(sc_pos, z) - vec3(0.0 , 0.0, 1.0)); 

    float lighting = max(0.0, dot(lightRay, -norm));    
    lighting *= 0.95;
    lighting += 0.2;    

    //light falloff by distance
    float c = length(sc_pos);
    c = 0.5/c;
    lighting *= clamp(c, 0.0, 1.0); 

    
    vec3 baseColor = vec3(0.4, 0.8, 0.9);  
    float alpha = 1.0;
    if (u_frag_use_texture) {  
        vec4 tex_color = texture(u_frag_texture, texture_coord);
        baseColor = tex_color.xyz;    
        alpha = tex_color.w;
    } else {   
        baseColor = color.xyz; 
    }  
    
    vec3 col = vec3(lighting)*baseColor;   
   
    // Output to screen    
    FragColor = vec4(vec3(col), 1.0);
}