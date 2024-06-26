#pragma once

#include "mandelbrot-shader-loader.h"

#include "math.h"
#include "renderer.h"

class mandelbrot_renderer: public gl::renderer { // Class that encapsulates position in mandelbrot set
protected:
    double& zoom;
    math::vec<double, 2>& position;

public:
    mandelbrot_renderer(double& new_zoom, math::vec<double, 2>& new_position)
        : zoom(new_zoom), position(new_position) {}

    virtual const char* get_backend_name() = 0;
    virtual void additional_ui() {}

    virtual ~mandelbrot_renderer() = default;
};
