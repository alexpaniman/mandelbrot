#shader vertex   ------------------------------------------------------------------------------------------
#version 440 core

layout(location = 0) in vec4 position;

void main() {
    gl_Position = position;
}

#shader fragment ------------------------------------------------------------------------------------------
#version 440 core

out vec4 color;

dvec3 calculate_mandelbrot(dvec2 p) {    
    dvec2 z = dvec2(0);  

    for (int i = 0; i < 128; ++i) {  
        z = dvec2(z.x * z.x - z.y * z.y, 2. * z.x * z.y) + p; 


        if (dot(z, z) > 4.0)
            return dvec3(sin(0.5 * i), sin(i + 5), cos(i * 0.4)) + dvec3(0.4);
    }

    return dvec3(0);
}

uniform double zoom;
uniform dvec2 position;

#define antialiasing_level 0

void main() {
    dvec2 resolution = dvec2(1920, 1080);
    dvec2 mandelbrot_position = (gl_FragCoord.xy / resolution.xy * 2. - 1.) *
        dvec2(resolution.x / resolution.y, 1) * zoom + position;

    // dvec3 output_color = vec3(0);

    // double e = 1.0 / min(resolution.y , resolution.x);    
    // for (double i = -antialiasing_level; i < antialiasing_level; ++i)
    //     for (double j = -antialiasing_level; j < antialiasing_level; ++j)
    // 		output_color += calculate_mandelbrot(mandelbrot_position + zoom * dvec2(i, j) *
    //                                              (e / antialiasing_level))
    //             / (4.0 * antialiasing_level * antialiasing_level);

    dvec3 output_color = calculate_mandelbrot(mandelbrot_position);

    color = vec4(output_color.xyz, 1);
}
