#pragma once

#include "opengl-setup.h"
#include "mandelbrot-renderer.h"
#include "colored-vertex.h"
#include "renderer.h"

class mandelbrot_cpu_arbitrary_renderer: public mandelbrot_renderer {
private:
    gl::vertex_vector_array<colored_vertex> points;
    gl::shaders::shader_program colored_renderer;

    int max_iterations = 512;
    int current_precision = 64;

    void draw_mandelbrot(math::vec<double, 2> position, double zoom);

public:
    using mandelbrot_renderer::mandelbrot_renderer;

    void setup() override;
    void draw()  override;

    void additional_ui() override;

    const char* get_backend_name() override;

    virtual ~mandelbrot_cpu_arbitrary_renderer() = default;
};
