#shader vertex   ------------------------------------------------------------------------------------------

#version 460 core

layout(location = 0) in vec4 position;

void main() {
    gl_Position = position;
}

#shader fragment ------------------------------------------------------------------------------------------

#version 460 core

out vec4 color;

#define ZOOM    1.4

vec3 fractal(vec2 p) {    
    vec2 z = vec2(0);  

    for (int i = 0; i < 128; ++i) {  
        z = vec2(z.x * z.x - z.y * z.y, 2. * z.x * z.y) + p; 

        if (dot(z,z) > 4.)
            return vec3(i, 1, 1) * 0.1;
    }

    return vec3(0);
}

void main() {
    vec2 iResolution = vec2(1920, 1080);
    vec2 c = (gl_FragCoord.xy / iResolution.xy * 2. - 1.)
        * vec2(iResolution.x / iResolution.y, 1)
        * ZOOM - vec2(.5,0.);

    color = vec4(fractal(c), 1);
}
