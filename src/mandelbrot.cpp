#include "gl-imgui.h"

// All available mandelbrot renderers
#include "mandelbrot-cpu-optimized-renderer.h"
#include "mandelbrot-cpu-unoptimized-renderer.h"
#include "mandelbrot-cpu-vectorized-renderer.h"
#include "mandelbrot-gpu-renderer.h"
#include "mandelbrot-gpu-single-precision-renderer.h"
#include "renderer.h"

#include <memory>
#include <vector>

class mandelbrot_window: public gl::renderer_handler_window {
private:
    // List of mandelbrot renderer's that can be switched easily
    std::vector<std::unique_ptr<mandelbrot_renderer>> backends;
    size_t current_backend_index;

    gl::imgui::imgui_renderer ui_renderer;

    // Zoom and position together determine location in mandelbrot:
    double zoom = 1.4;
    math::vec<double, 2> position { -0.5, 0.0 };

public:
    using gl::renderer_handler_window::renderer_handler_window;

    void on_key_pressed(gl::key pressed_key) override {
        using gl::key;

        switch (pressed_key) {
            case key::SPACE:
                ++ current_backend_index;
                if (current_backend_index >= backends.size())
                    current_backend_index = 0;

                ui_renderer.set_main_renderer(&*backends[current_backend_index]);
                break;

            // ---------------------------- ZOOM ----------------------------
            case key::KP_ADD:
            case key::EQUAL: zoom /= 1.2;                              break;

            case key::MINUS: zoom *= 1.2;                              break;

            // -------------------------- MOVEMENT --------------------------
            case key::DOWN:
            case key::S:
            case key::J:     position += math::vec(+0.0, -0.1) * zoom; break;

            case key::UP:
            case key::W:
            case key::K:     position += math::vec(+0.0, +0.1) * zoom; break;

            case key::LEFT:
            case key::H:
            case key::A:     position += math::vec(-0.1, +0.0) * zoom; break;

            case key::RIGHT:
            case key::L:
            case key::D:     position += math::vec(+0.1, +0.0) * zoom; break;

            // --------------------------------------------------------------
            default:                      /* Do nothing */             break;
        }
    }

private:
    template <typename mandelbrot_backend>
    void add_backend() {
        backends.push_back(std::make_unique<mandelbrot_backend>(zoom, position));
    }

public:
    void draw_ui() {
        ImGui::Begin("Name of backend");

        ImGui::Text("%s", backends[current_backend_index]->get_backend_name().c_str());
        ImGui::TextColored(ImVec4(1.0, 1.0, 0, 1), "FPS: %d", get_fps());

        ImGui::End();
    }

    void window_setup() override {
        add_backend<mandelbrot_gpu_renderer>();
        add_backend<mandelbrot_gpu_single_precision_renderer>();

        add_backend<mandelbrot_cpu_vectorized_renderer>();
        add_backend<mandelbrot_cpu_unoptimized_renderer>();

        add_backend<mandelbrot_cpu_optimized_renderer>();
        
        // Set UI drawing renderer from function
        ui_renderer.set_ui_renderer(std::bind(&mandelbrot_window::draw_ui, this));

        // ImGUI renderer will internally handle our renderer
        set_renderer(&ui_renderer);

        // First backend in the list, is first one to run
        current_backend_index = 0;
        ui_renderer.set_main_renderer(&*backends[current_backend_index]);
    }
};

int main(void) {
    mandelbrot_window window(1920, 1080, "Mandelbrot Set");
    window.draw_loop();
}
