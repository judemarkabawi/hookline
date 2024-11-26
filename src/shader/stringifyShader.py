import string

input = '''
#version 330
in vec2 texture_coord;
in vec4 color;
uniform float u_time;
uniform vec2 u_drawable_size"
uniform float u_size_ratio;

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

// The MIT License
// Copyright © 2019 Inigo Quilez
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

// Distance to a regular pentagon, without trigonometric functions. 

// List of some other 2D distances: https://www.shadertoy.com/playlist/MXdSRf
//
// and iquilezles.org/articles/distfunctions2d

float sdHexagram( in vec2 p, in float r )
{
    const vec4 k = vec4(-0.5,0.86602540378,0.57735026919,1.73205080757);
    
    p = abs(p);
    p -= 2.0*min(dot(k.xy,p),0.0)*k.xy;
    p -= 2.0*min(dot(k.yx,p),0.0)*k.yx;
    p -= vec2(clamp(p.x,r*k.z,r*k.w),r);
    return length(p)*sign(p.y);
}


void main()
{
    vec4 glow_col = color;
    
    // Normalized pixel coordinates (from 0 to 1)
    float minR = min(u_drawable_size.x, u_drawable_size.y);
    vec2 uvFixed = (gl_FragCoord.xy/minR)*2.0 - u_drawable_size.xy/minR;
    vec2 uvStretched = (gl_FragCoord.xy/u_drawable_size.xy)*2.0 - 1.0;
    
    float d = length(uvStretched);
    float d_fixed = length(uvFixed);
    
    float haloD = d - u_size_ratio;
    haloD = abs(haloD)/(1.25*u_size_ratio);
    haloD = 0.2/haloD;

    //little star effects
    float starR = u_size_ratio * 1.5;
    float starD = sdHexagram(uvFixed, starR);
    starD = abs(starD)/starR;
    starD = (sin(starD*(1.0/(3.0*starR)) - u_time*1.5) + 1.0)/1.5;
    starD = 0.3 / (starD+0.3);
    starD = pow(starD, 0.5);
    starD *= 0.4;
    
    float ITERATIONS = 6.0;
    for(float i = 0.0; i < ITERATIONS; i++) {
       float r = u_size_ratio;
       float s = 0.5;
       float twoPi = 2.0*3.131592;
       vec2 uvFixed0 = uvFixed;
       uvFixed0 *= s;
       float angle = twoPi*(i/ITERATIONS);
       uvFixed0 += vec2(cos(angle), sin(angle))*r;
       
        float newStarR = starR*s;
        float newStarD = sdHexagram(uvFixed0, newStarR);
        newStarD = abs(newStarD)/newStarR;
        newStarD = (sin(newStarD*(1.0/(3.0*newStarR)) - u_time*1.5 + 1.7) + 1.0)/1.5;
        newStarD = 0.3 / (newStarD+0.3);
        newStarD = pow(newStarD, 0.5);
        //newStarD *= 0.4;
        starD = max(starD, newStarD);
    }
    starD = clamp(starD, 0.0, 1.0);
    starD *= 0.6*min(exp(-5.0*(d_fixed-0.15)), 1.0);
    
    float bloomGlow = (sin(d*(1.0/u_size_ratio) - u_time) + 1.0)/2.0;
    bloomGlow = pow(bloomGlow, 1.0);
    bloomGlow = 0.5/(bloomGlow+0.6);
    bloomGlow = pow(bloomGlow, 2.0);
    bloomGlow = clamp(bloomGlow, 0.0, 1.0);
    bloomGlow *= 0.3;
    
    float lightness = bloomGlow + haloD + starD;
     
    vec3 hsv = rgb2hsv(glow_col.xyz);
    hsv.x += d*0.8 - starD*0.2;
    hsv.y *= 0.6;
    hsv.z *= 0.9;
    vec3 col = hsv2rgb(hsv);
    col += 0.1;
    col = vec3(pow(col.x, 0.9), pow(col.y, 0.9), pow(col.z, 0.9));
    
    
     
    col *= lightness;
    col *= (1.0 - pow((d_fixed+d)/2.0, 5.0)); //falloff towards edge
 
    // Output to screen
    FragColor = vec4(col, clamp(lightness, 0.0, 1.0));
}'''

ret_str = ""
for str in input.split('\n'):
    ret_str += '"'
    ret_str += str
    ret_str += '\\'
    ret_str += 'n'
    ret_str += '"'
    ret_str += '\n'


print(ret_str)
