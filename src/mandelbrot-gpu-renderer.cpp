#include "mandelbrot-gpu-renderer.h"
#include "math.h"
#include "opengl-setup.h"

std::string mandelbrot_gpu_renderer::get_backend_name() {
    return "MANDELBROT GPU RENDERER";
}

void mandelbrot_gpu_renderer::setup() {
    points.set_layout(math::vector_layout<double, 2>());

    points.assign_and_update({
            { -1.0, -1.0 },
            {  1.0, -1.0 },
            { -1.0,  1.0 },
            {  1.0,  1.0 },
    });

    mandelbrot_shader.from_file("res/mandelbrot.glsl");
}

void mandelbrot_gpu_renderer::draw() {
    mandelbrot_shader.uniform("zoom", zoom);
    mandelbrot_shader.uniform("position", position);

    gl::draw(gl::drawing_type::TRIANGLE_STRIP, points, mandelbrot_shader);
}
