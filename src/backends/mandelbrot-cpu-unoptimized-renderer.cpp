#include "mandelbrot-cpu-unoptimized-renderer.h"
#include "renderer.h"
#include "opengl-setup.h"
#include "math.h"
#include "colored-vertex.h"
#include "mandelbrot-gpu-renderer.h"
#include "mandelbrot-renderer.h"

#include <cmath>
#include <iostream>

const char* mandelbrot_cpu_unoptimized_renderer::get_backend_name() {
    return "MANDELBROT CPU UNOPTIMIZED RENDERER";
}

static math::vec<double, 3> calculate_mandelbrot_point(const math::vec<double, 2> point) {
    math::vec z(0.0, 0.0);

    for (int i = 0; i < 128; ++ i) {  
        z = math::vec(math::pow(z.x(), 2) - math::pow(z.y(), 2),
                      2.0 * z.x() * z.y()) + point;

        if (z.len() > 4.0)
            return math::vec((double) sin(0.5 * i) + 0.4,
                             (double) sin(i + 5.0) + 0.4,
                             (double) cos(i * 0.4) + 0.4);
    }

    return math::vec(0.0, 0.0, 0.0);
}

void mandelbrot_cpu_unoptimized_renderer::draw_mandelbrot(const math::vec<double, 2> position, const double zoom) {
    for (size_t i = 0; i < get_height(); ++ i)
        for (size_t j = 0; j < get_width(); ++ j) {
            colored_vertex& current = points[i * get_width() + j];

            // Current point's coordinates:
            current.point = math::vec(j * 2.0 / get_width() - 1.0, i * 2.0 / get_height() - 1.0);

            // std::cout << "un: num: " << (i * get_width() + j) << ", actual: " << current.point << "\n";

            math::vec<double, 2> mandelbrot_point =
                current.point * math::vec(get_width() / (double) get_height(), 1.0) * zoom + position;

            // Current point's color:
            current.color = calculate_mandelbrot_point(mandelbrot_point);
        }

    points.update();
}

void mandelbrot_cpu_unoptimized_renderer::setup() {
    points.set_layout(math::vector_layout<double, 2>() +
                      math::vector_layout<double, 3>());

    // Fill with point for each pixel:
    points.resize(get_width() * get_height());

    colored_renderer.from_file("res/gradient.glsl");
}

void mandelbrot_cpu_unoptimized_renderer::draw() {
    draw_mandelbrot(position, zoom);
    gl::draw(gl::drawing_type::POINTS, points, colored_renderer);
}
