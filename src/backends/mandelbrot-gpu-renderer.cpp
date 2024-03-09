#include "mandelbrot-gpu-renderer.h"
#include "gl-imgui.h"
#include "imgui.h"
#include "mandelbrot-shader-loader.h"
#include <iostream>

void mandelbrot_gpu_renderer::additional_ui() {
    ImGui::SliderInt("Antialiasing level", &antialiasing_level, 0, 2);
}

const char* mandelbrot_gpu_renderer::get_backend_name() {
    return "MANDELBROT GPU RENDERER";
}

void mandelbrot_gpu_renderer::setup() {
    // Set default antialiasing level
    antialiasing_level = 1;

    points.set_layout(math::vector_layout<double, 2>());
    points.assign_and_update({
            { -1.0, -1.0 },
            {  1.0, -1.0 },
            { -1.0,  1.0 },
            {  1.0,  1.0 },
    });

    load_mandelbrot_shader(mandelbrot_shader, "mandelbrot");
}

void mandelbrot_gpu_renderer::draw() {
    mandelbrot_shader.uniform("resolution",
                              math::vec((double) get_width(), (double) get_height()));

    mandelbrot_shader.uniform("zoom", zoom);
    mandelbrot_shader.uniform("position", position);

    mandelbrot_shader.uniform("antialiasing_level",
                              (double) antialiasing_level);

    gl::draw(gl::drawing_type::TRIANGLE_STRIP, points, mandelbrot_shader);
}
