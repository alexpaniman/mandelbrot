#pragma once

#include "opengl-setup.h"
#include "mandelbrot-renderer.h"
#include "colored-vertex.h"
#include "renderer.h"

class mandelbrot_gpu_renderer: public mandelbrot_renderer {
private:
    gl::vertex_vector_array<math::vec<double, 2>> points;
    gl::shaders::shader_program mandelbrot_shader;

public:
    using mandelbrot_renderer::mandelbrot_renderer;

    void setup() override;
    void draw()  override;

    virtual ~mandelbrot_gpu_renderer() = default;
};
