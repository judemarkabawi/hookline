#version 330
in vec2 texture_coord;
in vec4 color;
uniform float u_time;
uniform float u_size_ratio;
uniform vec2 u_movement_vector;
uniform float u_age; 
out vec4 FragColor; 


//conversion functions from
//https://gist.github.com/983/e170a24ae8eba2cd174f
vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

float sdParabolaSeg( in vec2 pos, in float wi, in float he )
{
    pos.x = abs(pos.x);
    float ik = wi*wi/he;
    float p = ik*(he-pos.y-0.5*ik)/3.0;
    float q = pos.x*ik*ik*0.25;
    float h = q*q - p*p*p;
    float r = sqrt(abs(h));
    float x = (h>0.0) ? 
        pow(q+r,1.0/3.0) - pow(abs(q-r),1.0/3.0)*sign(r-q) :
        2.0*cos(atan(r/q)/3.0)*sqrt(p);
    x = min(x,wi);
    return length(pos-vec2(x,he-x*x/ik)) * 
           sign(ik*(pos.y-he)+pos.x*pos.x);
}

float sdParabola( in vec2 pos, in float k )
{
    pos.x = abs(pos.x);
    float ik = 1.0/k;
    float p = ik*(pos.y - 0.5*ik)/3.0;
    float q = 0.25*ik*ik*pos.x;
    float h = q*q - p*p*p;
    float r = sqrt(abs(h));
    float x = (h>0.0) ? 
        pow(q+r,1.0/3.0) - pow(abs(q-r),1.0/3.0)*sign(r-q) :
        2.0*cos(atan(r,q)/3.0)*sqrt(p);
    return length(pos-vec2(x,k*x*x)) * sign(pos.x-x);
}

float atan2(float y, float x) { return mod(atan(y,x) + 3.141592, 2.0*3.141592); }


void main()
{    
    vec2 uv = texture_coord*2.0 - 1.0;
    
    
    float dist = length(uv);
    float angle = atan2(-u_movement_vector.x, -u_movement_vector.y);
    
    float PI = 3.141592;
    vec2 oldUV = uv;
    uv.x = cos(angle)*oldUV.x - sin(angle)*oldUV.y;
    uv.y = sin(angle)*oldUV.x + cos(angle)*oldUV.y;
   
    
    
    vec2 uv0 = uv;
    float size_inv = 1.0/u_size_ratio;
    uv *= size_inv;
    uv.y -=  u_size_ratio;
    float wave = (sin(uv.y*5.0 + 20.0*u_time - 10.0*cos(uv.x))+1.0)*0.3;

    float d = sdParabola(uv, -u_size_ratio*3.0)/(u_size_ratio);
    wave *= (0.5 - d);
    wave = 0.05 / wave;
    float p = d;
    p = abs(p);
    p = exp(-u_size_ratio*(p-0.3)*2.0);
    
    float falloff = 2.0*u_size_ratio - pow(length(uv0 ), 0.8);
    p *= falloff;
    p += max(-d*0.7, -0.4)*p;
    
    p += 0.0075/length(oldUV)/u_size_ratio;
   
    
     wave *= p;
     p += wave;
     
     float falloff2 = max(1.0 - min(pow(length(oldUV), 2.0), 1.0), 0.0);
     p *= falloff2;
     
     vec3 hsv = rgb2hsv(color.xyz);
     hsv.x += -0.1 + dist*u_size_ratio*0.4;
     hsv.y *= 0.8;
     vec3 col = hsv2rgb(hsv);
     col += 0.3;
     
     
     col *= p;
    col *= 1.0 - smoothstep(0.95, 1.0, u_age);
    // Output to screen
    FragColor = vec4(col, 1.0);
}