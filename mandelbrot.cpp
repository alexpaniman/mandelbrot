#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstddef>
#include <iostream>
#include <vector>
#include <cmath>

#include "opengl-setup.h"
#include "opengl-wrapper.h"
#include "math.h"
#include "vertex-array.h"
#include "vertex-buffer.h"
#include "vertex-layout.h"
#include "vertex-vector-array.h"

class mandelbrot_window: public gl::window {
private:
    struct colored_vertex final {
        math::vec<double, 2> point;
        math::vec<double, 3> color;

        colored_vertex()
            : point(0.0, 0.0), color(0.0, 0.0, 0.0) {};

        colored_vertex(math::vec<double, 2> new_point,
                       math::vec<double, 3> new_color)
            : point(new_point), color(new_color) {};
    };

    gl::vertex_vector_array<colored_vertex> points;
    gl::shaders::shader_program colored_renderer;


    math::vec<double, 3> calculate_mandelbrot_point(math::vec<double, 2> point) {
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

    void draw_mandelbrot(math::vec<double, 2> position, const double zoom) {
        for (size_t i = 0; i < height; ++ i)
            for (size_t j = 0; j < width; ++ j) {
                colored_vertex& current = points[i * width + j];

                // Current point's coordinates:
                current.point = math::vec(j * 2.0 / width - 1.0, i * 2.0 / height - 1.0);

                math::vec<double, 2> mandelbrot_point =
                    current.point * math::vec(width / (double) height, 1.0) * zoom + position;

                // Current point's color:
                current.color = calculate_mandelbrot_point(mandelbrot_point);
            }

        points.update();
    }

    double zoom = 1.4;
    math::vec<double, 2> position { -0.5, 0.0 };

public:
    using gl::window::window;

    void on_key_pressed(key pressed_key) override {
        switch (pressed_key) {
        case gl::window::key::KEY_KP_ADD:
        case gl::window::key::KEY_EQUAL : zoom /= 1.2;                              break;
        case gl::window::key::KEY_MINUS : zoom *= 1.2;                              break;

        case gl::window::key::KEY_DOWN  :
        case gl::window::key::KEY_S     :
        case gl::window::key::KEY_J     : position += math::vec(+0.0, -0.3) * zoom; break;

        case gl::window::key::KEY_UP    :
        case gl::window::key::KEY_W     :
        case gl::window::key::KEY_K     : position += math::vec(+0.0, +0.3) * zoom; break;

        case gl::window::key::KEY_LEFT  :
        case gl::window::key::KEY_H     :
        case gl::window::key::KEY_A     : position += math::vec(-0.3, +0.0) * zoom; break;

        case gl::window::key::KEY_RIGHT :
        case gl::window::key::KEY_L     :
        case gl::window::key::KEY_D     : position += math::vec(+0.3, +0.0) * zoom; break;

        default:
            break;
        }
    }

    void setup() override {
        points.set_layout(math::vector_layout<double, 2>() +
                          math::vector_layout<double, 3>());

        // Fill with point for each pixel:
        points.resize((size_t) width * (size_t) height);

        colored_renderer.from_file("res/gradient.glsl");
    }

    void on_fps_updated() override {
        std::cout << "FPS: " << get_fps() << std::endl;
    }

    void draw() override {
        draw_mandelbrot(position, zoom);
        gl::draw(gl::drawing_type::POINTS, points, colored_renderer);
    }
};

int main(void) {
    mandelbrot_window window(1920, 1080, "Mandelbrot Illustrator");
    window.draw_loop();
}
