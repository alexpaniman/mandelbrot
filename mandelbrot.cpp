#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "opengl-setup.h"
#include "math.h"
#include "vertex-array.h"
#include "vertex-buffer.h"
#include "vertex-layout.h"

class mandelbrot_window: public gl::window {
private:
    // gl::vertex_buffer* bf;
    // gl::vertex_array* array;
    
    // gl::shaders::shader_program colored_renderer {};

    void draw_mandelbrot() {
        // gl::vertex_array<math::vec<double, 2>> layout;
    }

public:
    using gl::window::window;

    unsigned int m_buffer_id;

    void setup() override {
        static math::vec<double, 2> points[] = {
            {  0.0,  1.0 },
            { -0.5, -0.5 },
            {  1.0, -1.0 },
        };

        glGenBuffers(1, &m_buffer_id);

        glBindBuffer(GL_ARRAY_BUFFER, m_buffer_id);
        glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);


        // static gl::vertex_buffer bf({ points, sizeof(points) });
        // static gl::vertex_array arr(bf, gl::layout<math::dvec2>());

        // this->bf = &bf; this->array = &arr;

        // colored_renderer.from_file("res/basic.shader");
        // colored_renderer.bind();
    }

    void draw()  override {
        glDrawBuffers(GL_TRIANGLES, &m_buffer_id);
    }
};

int main(void) {
    mandelbrot_window window(1920, 1080, "Mandelbrot Illustrator");
    window.draw_loop();
}
