#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include "opengl-setup.h"
#include "opengl-wrapper.h"
#include "math.h"
#include "vertex-array.h"
#include "vertex-buffer.h"
#include "vertex-layout.h"
#include "vertex-vector-array.h"

class mandelbrot_window: public gl::window {
private:
    struct colored_point final {
        math::vec<float, 2> point;
        math::vec<float, 3> color;
    };

    gl::vertex_vector_array<colored_point> points;
    gl::shaders::shader_program colored_renderer;

public:
    using gl::window::window;

    void setup() override {
        points.set_layout(math::vector_layout<float, 2>() +
                          math::vector_layout<float, 3>());

        points.assign_and_update({
                { { -1.0f, -1.0f }, { 1.0, 0.0, 0.0 } },
                { {  1.0f, -1.0f }, { 0.0, 0.0, 1.0 } },
                { { -1.0f,  1.0f }, { 0.0, 1.0, 0.0 } },
                { {  1.0f,  1.0f }, { 0.0, 0.0, 1.0 } },
        });

        colored_renderer.from_file("res/mandelbrot.shader");
    }

    void draw() override {
        gl::draw(gl::drawing_type::TRIANGLE_STRIP, points, colored_renderer);
    }
};

int main(void) {
    mandelbrot_window window(1920, 1080, "Mandelbrot Illustrator");
    window.draw_loop();
}
