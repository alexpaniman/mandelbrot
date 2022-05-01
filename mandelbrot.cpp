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
        math::vec<float, 2> point;
        math::vec<float, 3> color;

        colored_vertex()
            : point(0.0f, 0.0f), color(0.0f, 0.0f, 0.0f) {};

        colored_vertex(math::vec<float, 2> new_point,
                       math::vec<float, 3> new_color)
            : point(new_point), color(new_color) {};
    };

    gl::vertex_vector_array<colored_vertex> points;
    gl::shaders::shader_program colored_renderer;


    static inline float power2(float value) {
        return value * value;
    }

    static inline float dot(math::vec<float, 2> vec) {
        return power2(vec.x()) + power2(vec.y());
    }

    math::vec<float, 3> calculate_mandelbrot_point(math::vec<float, 2> point) {
        math::vec<float, 2> iteration(0.0f, 0.0f);

        for (int i = 0; i < 128; ++ i) {  
            iteration = math::vec(power2(iteration.x()) - power2(iteration.y()),
                                  2.0f * iteration.x() * iteration.y());

            iteration.x() += point.x(); iteration.y() += point.y();
            if (dot(iteration) > 4.0f)
                return math::vec((float) sin(0.5f * i) + 0.4f,
                                 (float) sin(i + 5.0f) + 0.4f,
                                 (float) cos(i * 0.4f) + 0.4f);
        }

        return math::vec(0.0f, 0.0f, 0.0f);
    }

    void draw_mandelbrot(math::vec<float, 2> position, const double zoom) {
        for (size_t i = 0; i < height; ++ i)
            for (size_t j = 0; j < width; ++ j) {
                colored_vertex& current = points[i * width + j];

                // Current point's coordinates:
                current.point = math::vec(j * 2.0f / width - 1.0f, i * 2.0f / height - 1.0f);

                math::vec<float, 2> mandelbrot_point =
                    current.point * math::vec(width / (float) height, 1.0f) * zoom + position;

                // Current point's color:
                current.color = calculate_mandelbrot_point(mandelbrot_point);
            }

        points.update();
    }

public:
    using gl::window::window;

    void setup() override {
        points.set_layout(math::vector_layout<float, 2>() +
                          math::vector_layout<float, 3>());

        // Fill with point for each pixel:
        points.resize((size_t) width * (size_t) height);

        colored_renderer.from_file("res/gradient.glsl");
    }

    void on_fps_updated() override {
        std::cout << "FPS: " << get_fps() << std::endl;
    }

    void draw() override {
        draw_mandelbrot(math::vec(-0.5f, 0.0f), 1.4f);
        gl::draw(gl::drawing_type::POINTS, points, colored_renderer);
    }
};

int main(void) {
    mandelbrot_window window(1920, 1080, "Mandelbrot Illustrator");
    window.draw_loop();
}
