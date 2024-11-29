#version 330
in vec2 texture_coord;
in vec4 color;
uniform float u_time; 
uniform float u_health; 
out vec4 FragColor;
float atan2(float y, float x) { return mod(atan(y,x) + 3.141592, 2.0*3.141592); }
void main()    
{  
    // Output to screen    
    vec2 uv = texture_coord*2.0 - 1.0;
    float angle = atan2(uv.y, uv.x);
    angle += 3.141592;
    angle /= 2.0*3.141592;
    FragColor = vec4(vec3(float(angle <= u_health)), 1.0);
}