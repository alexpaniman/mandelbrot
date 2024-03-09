#include "mandelbrot-cpu-vectorized-renderer.h"
#include "renderer.h"
#include "opengl-setup.h"
#include "math.h"
#include "colored-vertex.h"
#include "mandelbrot-gpu-renderer.h"
#include "mandelbrot-renderer.h"

#include <cmath>
#include <cstring>
#include <iostream>

constexpr int batch_size = 4;

const char* mandelbrot_cpu_vectorized_renderer::get_backend_name() {
    return "MANDELBROT CPU VECTORIZED RENDERER";
}

static void calculate_mandelbrot_point(const double x[batch_size], const double y[batch_size],
                                       double output_color[batch_size][3]) {

    math::vec z(0.0, 0.0);

    double z_x[] = { 0.0, 0.0, 0.0, 0.0 };
    double z_y[] = { 0.0, 0.0, 0.0, 0.0 };

    for (int i = 0; i < 128; ++ i) {  
        double saved_x[batch_size] = { z_x[0], z_x[1], z_x[2], z_x[3] };

        for (int i = 0; i < batch_size; ++ i) z_x[i] *= z_x[i];           // new_z_x = z_x^2

        double square_y[batch_size] = { z_y[0], z_y[1], z_y[2], z_y[3] };
        for (int i = 0; i < batch_size; ++ i) square_y[i] *= square_y[i];

        for (int i = 0; i < batch_size; ++ i) z_x[i] -= square_y[i];      // new_z_x = z_x^2 - z_y^2

        for (int i = 0; i < batch_size; ++ i) z_y[i] *= saved_x[i];       // new_z_y = z_x * z_y
        for (int i = 0; i < batch_size; ++ i) z_y[i] *= 2.0;              // new_z_y = 2.0 * z_x * z_y

        for (int i = 0; i < batch_size; ++ i) z_x[i] += x[i];             // new_z_x = z_x^2 - z_y^2 + x
        for (int i = 0; i < batch_size; ++ i) z_y[i] += y[i];             // new_z_y = 2.0 * z_x * z_y + y

        z = math::vec(math::pow(z.x(), 2) - math::pow(z.y(), 2), 2.0 * z.x() * z.y()) +
            math::vec(x[0], y[0]);

        // std::cout << "vector: " << z << " <=> (" << z_x[0] << ", " << z_y[0] << ")" << std::endl;

        double saved_y[batch_size] = { z_y[0], z_y[1], z_y[2], z_y[3] };
        double result[] = { z_x[0], z_x[1], z_x[2], z_x[3] };
        
        for (int i = 0; i < batch_size; ++ i) result[i] *= result[i];     // new_z_x = (z_x^2 - z_y^2 + x)^2
        for (int i = 0; i < batch_size; ++ i) saved_y[i] *= saved_y[i];   // new_z_y = (2.0 * z_x * z_y + y)^2
        
        for (int i = 0; i < batch_size; ++ i) result[i] += saved_y[i];    // new_z_x = (z_x^2 - z_y^2 + x)^2 +
                                                                          //           (2.0 * z_x * z_y + y)^2
        
        bool all_finished = true;
        for (int j = 0; j < batch_size; ++ j)
            if (result[j] <= 4.0)
                all_finished = false;
            else {
                if (output_color[j][0] == 0.0) {
                    output_color[j][0] = sin(0.5 * i);
                    output_color[j][1] = sin(5.0 + i);
                    output_color[j][2] = cos(0.4 * i);

                    for (int q = 0; q < 3; ++ q)
                        output_color[j][q] += 0.4;
                }
            }


        if (all_finished)
            return;
    }
}

void mandelbrot_cpu_vectorized_renderer::draw_mandelbrot(const math::vec<double, 2> position, const double zoom) {
    int index = 0; // Current point index

    for (size_t i = 0; i < get_height(); ++ i)
        for (size_t j = 0; j < get_width(); j += batch_size, index += batch_size) {
            double x[] = {        0.0,        1.0,        2.0,        3.0 };
            for (int i = 0; i < batch_size; ++ i) x[i] += j;

            double y[] = { (double) i, (double) i, (double) i, (double) i };
            for (int i = 0; i < batch_size; ++ i) x[i] *= 2.0;
            for (int i = 0; i < batch_size; ++ i) y[i] *= 2.0;

            for (int i = 0; i < batch_size; ++ i) x[i] /= get_width();
            for (int i = 0; i < batch_size; ++ i) y[i] /= get_height();

            for (int i = 0; i < batch_size; ++ i) -- x[i];
            for (int i = 0; i < batch_size; ++ i) -- y[i];

            // Current point's coordinates:
            // current.point = math::vec(j * 2.0 / get_width() - 1.0, i * 2.0 / get_height() - 1.0);

            for (int i = 0; i < batch_size; ++ i) points[index + i].point.x() = x[i];
            for (int i = 0; i < batch_size; ++ i) points[index + i].point.y() = y[i];

            // for (int i = 0; i < batch_size; ++ i)
                // std::cout << "on: num: " << (index + i) << ", actual: (" << x[i] << "," << y[i] << ")\n";

            double ratio = get_width() / (double) get_height();
            for (int i = 0; i < batch_size; ++ i) x[i] *= ratio;

            for (int i = 0; i < batch_size; ++ i) x[i] *= zoom;
            for (int i = 0; i < batch_size; ++ i) y[i] *= zoom;

            for (int i = 0; i < batch_size; ++ i) x[i] += position[0];
            for (int i = 0; i < batch_size; ++ i) y[i] += position[1];

            double output_color[batch_size][3] = {
                { 0.0, 0.0, 0.0 },
                { 0.0, 0.0, 0.0 },
                { 0.0, 0.0, 0.0 },
                { 0.0, 0.0, 0.0 }
            };

            calculate_mandelbrot_point(x, y, output_color);

            for (int j = 0; j < 3; ++ j)
                for (int i = 0; i < batch_size; ++ i)
                    points[index + i].color[j] = output_color[i][j];
        }

    points.update();
}

void mandelbrot_cpu_vectorized_renderer::setup() {
    points.set_layout(math::vector_layout<double, 2>() +
                      math::vector_layout<double, 3>());

    // Fill with point for each pixel:
    points.resize(get_width() * get_height());

    load_mandelbrot_shader(colored_renderer, "gradient");
}

void mandelbrot_cpu_vectorized_renderer::draw() {
    draw_mandelbrot(position, zoom);
    gl::draw(gl::drawing_type::POINTS, points, colored_renderer);
}
