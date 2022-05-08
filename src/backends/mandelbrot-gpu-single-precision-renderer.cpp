#include "mandelbrot-gpu-single-precision-renderer.h"
#include "math.h"
#include "opengl-setup.h"

std::string mandelbrot_gpu_single_precision_renderer::get_backend_name() {
    return "MANDELBROT GPU SINGLE PRECISION RENDERER";
}

void mandelbrot_gpu_single_precision_renderer::setup() {
    points.set_layout(math::vector_layout<double, 2>());

    points.assign_and_update({
            { -1.0, -1.0 },
            {  1.0, -1.0 },
            { -1.0,  1.0 },
            {  1.0,  1.0 },
    });

    mandelbrot_shader.from_file("res/mandelbrot-single-precision.glsl");
}

void mandelbrot_gpu_single_precision_renderer::draw() {
    mandelbrot_shader.uniform("zoom", (float) zoom);
    mandelbrot_shader.uniform("position", math::vec((float) position.x(),
                                                    (float) position.y()));

    gl::draw(gl::drawing_type::TRIANGLE_STRIP, points, mandelbrot_shader);
}
