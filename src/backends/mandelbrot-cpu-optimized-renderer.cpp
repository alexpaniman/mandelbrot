#include "mandelbrot-cpu-optimized-renderer.h"
#include "renderer.h"
#include "opengl-setup.h"
#include "math.h"
#include "colored-vertex.h"
#include "mandelbrot-gpu-renderer.h"
#include "mandelbrot-renderer.h"

#include <cmath>
#include <cstring>
#include <immintrin.h>
#include <iostream>

static constexpr int batch_size = 4;

const char* mandelbrot_cpu_optimized_renderer::get_backend_name() {
    return "MANDELBROT CPU OPTIMIZED RENDERER";
}

static void calculate_mandelbrot_point(__m256d x, __m256d y, colored_vertex vertices_to_color[batch_size]) {
    // Mark all verticies as uncolored (0.0 in R chanell is used to represent that),
    // check out color setting part below, which relies on this for more information.
    for (int i = 0; i < batch_size; ++ i)
        vertices_to_color[i].color.r() = 0.0;

    __m256d z_x = _mm256_set1_pd(0.0);
    __m256d z_y = _mm256_set1_pd(0.0);

    for (int i = 0; i < 128; ++ i) {  
        __m256d saved_x = z_x;
        z_x = _mm256_mul_pd(z_x, z_x);

        __m256d square_y = _mm256_mul_pd(z_y, z_y);
        z_x = _mm256_sub_pd(z_x, square_y);

        z_y = _mm256_mul_pd(z_y, saved_x);
        z_y = _mm256_mul_pd(z_y, _mm256_set1_pd(2.0));

        z_x = _mm256_add_pd(z_x, x);
        z_y = _mm256_add_pd(z_y, y);

        __m256d saved_y = z_y, length_squared = z_x;
        
        length_squared = _mm256_mul_pd(length_squared, length_squared);
        saved_y = _mm256_mul_pd(saved_y, saved_y);

        length_squared = _mm256_add_pd(length_squared, saved_y);

        bool entire_batch_finished = true;
        for (int j = 0; j < batch_size; ++ j)
            if (length_squared[j] <= 4.0)
                entire_batch_finished = false;
            else {
                math::vec<double, 3>& current_color = vertices_to_color[j].color;

                // r == 0.0 is used to indicate that vertex wasn't colored as
                // sin(0.5 * i) + 0.4 is never equal to 0.4 for any integer i
                if (current_color.r() == 0.0) {
                    // This is necessary as color depends on the exact number of
                    // iterations after which vertex escapes from mandelbrot set

                    current_color.r() = sin(0.5 * i) + 0.4;
                    current_color.g() = sin(5.0 + i) + 0.4;
                    current_color.b() = cos(0.4 * i) + 0.4;
                }
            }


        if (entire_batch_finished)
            return;
    }

    // Mark verticies in mandelbrot set with their own color
    for (int i = 0; i < batch_size; ++ i)
        // Same trick as above to check is vertex colored or not
        if (vertices_to_color[i].color.r() == 0.0)
            vertices_to_color[i].color = math::vec(0.0, 0.0, 0.0);
}

void mandelbrot_cpu_optimized_renderer::draw_mandelbrot(const math::vec<double, 2> position, const double zoom) {
    int index = 0; // Current point index

    for (size_t i = 0; i < get_height(); ++ i)
        for (size_t j = 0; j < get_width(); j += batch_size, index += batch_size) {
            __m256d x = _mm256_set_pd(0.0, 1.0, 2.0, 3.0);
            x = _mm256_add_pd(x, _mm256_set1_pd(j));

            __m256d y = _mm256_set1_pd((double) i);
            x = _mm256_mul_pd(x, _mm256_set1_pd(2.0 / (double) get_width()));
            y = _mm256_mul_pd(y, _mm256_set1_pd(2.0 / (double) get_height()));

            x = _mm256_sub_pd(x, _mm256_set1_pd(1.0));
            y = _mm256_sub_pd(y, _mm256_set1_pd(1.0));

            for (int i = 0; i < batch_size; ++ i)
                points[index + i].point.x() = ((double*) &x)[i];

            for (int i = 0; i < batch_size; ++ i)
                points[index + i].point.y() = ((double*) &y)[i];

            double ratio = get_width() / (double) get_height();
            x = _mm256_mul_pd(x, _mm256_set1_pd(ratio));

            x = _mm256_mul_pd(x, _mm256_set1_pd(zoom));
            y = _mm256_mul_pd(y, _mm256_set1_pd(zoom));

            x = _mm256_add_pd(x, _mm256_set1_pd(position[0]));
            y = _mm256_add_pd(y, _mm256_set1_pd(position[1]));

            calculate_mandelbrot_point(x, y, &points[index]);
        }

    points.update();
}

void mandelbrot_cpu_optimized_renderer::setup() {
    points.set_layout(math::vector_layout<double, 2>() +
                      math::vector_layout<double, 3>());

    // Fill with point for each pixel:
    points.resize(get_width() * get_height());

    colored_renderer.from_file("res/gradient.glsl");
}

void mandelbrot_cpu_optimized_renderer::draw() {
    draw_mandelbrot(position, zoom);
    gl::draw(gl::drawing_type::POINTS, points, colored_renderer);
}
