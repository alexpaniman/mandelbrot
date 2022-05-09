#include "mandelbrot-gpu-single-precision-renderer.h"
#include "gl-imgui.h"

void mandelbrot_gpu_single_precision_renderer::additional_ui() {
    ImGui::SliderInt("Antialiasing level", &antialiasing_level, 0, 2);
}

const char* mandelbrot_gpu_single_precision_renderer::get_backend_name() {
    return "MANDELBROT GPU SINGLE PRECISION RENDERER";
}

void mandelbrot_gpu_single_precision_renderer::setup() {
    // Set default antialiasing level:
    antialiasing_level = 2;

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
    mandelbrot_shader.uniform("resolution",
                              math::vec((float) get_width(), (float) get_height()));

    mandelbrot_shader.uniform("zoom", (float) zoom);
    mandelbrot_shader.uniform("position", math::vec((float) position.x(),
                                                    (float) position.y()));

    mandelbrot_shader.uniform("antialiasing_level", (float) antialiasing_level);

    gl::draw(gl::drawing_type::TRIANGLE_STRIP, points, mandelbrot_shader);
}
