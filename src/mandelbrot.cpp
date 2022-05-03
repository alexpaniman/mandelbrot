#include "colored-vertex.h"
#include "mandelbrot-cpu-unoptimized-renderer.h"
#include "mandelbrot-gpu-renderer.h"
#include "mandelbrot-renderer.h"
#include "opengl-setup.h"
#include "opengl-wrapper.h"
#include "math.h"
#include "renderer-handler-window.h"
#include "vertex-array.h"
#include "vertex-buffer.h"
#include "vertex-layout.h"
#include "vertex-vector-array.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstddef>
#include <iostream>
#include <vector>
#include <memory>
#include <cmath>

class mandelbrot_window: public gl::renderer_handler_window {
private:
    std::vector<std::unique_ptr<mandelbrot_renderer>> backends;
    size_t current_backend;

    double zoom = 1.4;
    math::vec<double, 2> position { -0.5, 0.0 };

public:
    using gl::renderer_handler_window::renderer_handler_window;

    void on_key_pressed(gl::key pressed_key) override {
        using gl::key;

        switch (pressed_key) {
            case key::SPACE:
                ++ current_backend;
                if (current_backend >= backends.size())
                    current_backend = 0;

                set_renderer(&*backends[current_backend]);
                break;

            // ---------------------------- ZOOM ----------------------------
            case key::KP_ADD:
            case key::EQUAL: zoom /= 1.2;                              break;

            case key::MINUS: zoom *= 1.2;                              break;

            // -------------------------- MOVEMENT --------------------------
            case key::DOWN:
            case key::S:
            case key::J:     position += math::vec(+0.0, -0.3) * zoom; break;

            case key::UP:
            case key::W:
            case key::K:     position += math::vec(+0.0, +0.3) * zoom; break;

            case key::LEFT:
            case key::H:
            case key::A:     position += math::vec(-0.3, +0.0) * zoom; break;

            case key::RIGHT:
            case key::L:
            case key::D:     position += math::vec(+0.3, +0.0) * zoom; break;

            // --------------------------------------------------------------
            default:                      /* Do nothing */             break;
        }
    }

    void on_fps_updated() override {
        std::cout << "FPS: " << get_fps() << std::endl;
    }

private:
    template <typename mandelbrot_backend>
    void add_backend() {
        backends.push_back(std::make_unique<mandelbrot_backend>(zoom, position));
    }

public:
    void window_setup() override {
        add_backend<mandelbrot_gpu_renderer>();
        add_backend<mandelbrot_cpu_unoptimized_renderer>();

        current_backend = 0;
        set_renderer(&*backends.front()); // Initialize with first backend
    }

    virtual ~mandelbrot_window() = default;
};

int main(void) {
    mandelbrot_window window(1920, 1080, "Mandelbrot Illustrator");
    window.draw_loop();
}
