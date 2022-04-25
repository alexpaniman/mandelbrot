#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include "opengl-setup.h"
#include "math.h"
#include "vertex-array.h"
#include "vertex-buffer.h"
#include "vertex-layout.h"

class mandelbrot_window: public gl::window {
private:
    struct colored_point final {
        math::vec<float, 2> point;
        math::vec<float, 3> color;
    };

    std::vector<colored_point> points;
    gl::vertex_array array;

    gl::shaders::shader_program colored_renderer;

public:
    using gl::window::window;

    void setup() override {
        points.assign({
                { {  0.0,  0.5 }, { 1.0, 0.0, 0.0 } },
                { {  0.5, -0.5 }, { 0.0, 1.0, 0.0 } },
                { { -0.5, -0.5 }, { 0.0, 0.0, 1.0 } }
        });

        array.assign(math::vector_layout<float, 2>() +
                     math::vector_layout<float, 3>(), points);

        colored_renderer.from_file("res/basic.shader");
    }

    void draw() override {
        gl::draw(gl::drawing_type::TRIANGLES, array, colored_renderer);
    }
};

int main(void) {
    mandelbrot_window window(1920, 1080, "Mandelbrot Illustrator");
    window.draw_loop();
}
