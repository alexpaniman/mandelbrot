#shader vertex   ------------------------------------------------------------------------------------------
#version 330 core

layout(location = 0) in vec4 position;

void main() {
    gl_Position = position;
}

#shader fragment ------------------------------------------------------------------------------------------
#version 330 core

out vec4 color;

#define ZOOM    1.4

vec3 fractal(vec2 p) {    
    vec2 z = vec2(0);  

    for (int i = 0; i < 128; ++i) {  
        z = vec2(z.x * z.x - z.y * z.y, 2. * z.x * z.y) + p; 


        if (dot(z, z) > 4.0f) {
            vec3 color = vec3(sin(0.5 * i), sin(i + 5), cos(i * 0.4)) + vec3(0.4f);


            return color;
        }
        // if (dot(z,z) > 4.)
        //     return vec3(i, 1, 1) * 0.1;
    }

    return vec3(0);
}

#define AA 3

void main() {
    vec2 iResolution = vec2(1920, 1080);
    vec2 c = (gl_FragCoord.xy / iResolution.xy * 2. - 1.) * vec2(iResolution.x / iResolution.y, 1) * ZOOM - vec2(.5,0.);

    vec3 col = vec3(0);

    float e = 1. / min(iResolution.y , iResolution.x);    
    for (float i = -AA; i < AA; ++i) {
        for (float j = -AA; j < AA; ++j) {
    		col += fractal(c + ZOOM * vec2(i, j) * (e/AA)) / (4.*AA*AA);
        }
    }

    color = vec4(col, 1);
}
