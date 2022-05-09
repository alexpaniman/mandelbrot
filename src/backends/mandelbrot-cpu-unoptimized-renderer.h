#pragma once

#include "opengl-setup.h"
#include "mandelbrot-renderer.h"
#include "colored-vertex.h"
#include "renderer.h"

class mandelbrot_cpu_unoptimized_renderer: public mandelbrot_renderer {
private:
    gl::vertex_vector_array<colored_vertex> points;
    gl::shaders::shader_program colored_renderer;

    void draw_mandelbrot(math::vec<double, 2> position, double zoom);

public:
    using mandelbrot_renderer::mandelbrot_renderer;

    void setup() override;
    void draw()  override;

    const char* get_backend_name() override;

    virtual ~mandelbrot_cpu_unoptimized_renderer() = default;
};
