#pragma once

#include "opengl-setup.h"
#include "mandelbrot-renderer.h"
#include "colored-vertex.h"
#include "renderer.h"

class mandelbrot_gpu_single_precision_renderer: public mandelbrot_renderer {
private:
    gl::vertex_vector_array<math::vec<double, 2>> points;
    gl::shaders::shader_program mandelbrot_shader;

    int antialiasing_level;

public:
    using mandelbrot_renderer::mandelbrot_renderer;

    void setup() override;
    void draw()  override;

    void additional_ui() override;
    const char* get_backend_name() override;

    virtual ~mandelbrot_gpu_single_precision_renderer() = default;
};
