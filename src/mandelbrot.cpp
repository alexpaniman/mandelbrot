#include "gl-imgui.h"

// All available mandelbrot renderers
#include "mandelbrot-cpu-optimized-renderer.h"
#include "mandelbrot-cpu-unoptimized-renderer.h"
#include "mandelbrot-cpu-vectorized-renderer.h"
#include "mandelbrot-gpu-renderer.h"
#include "mandelbrot-gpu-single-precision-renderer.h"

#include <cstdlib>
#include <iostream>
#include <queue>
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

    void set_backend_by_index() {
        ui_renderer.set_main_renderer(&*backends[current_backend_index]);
    }

public:
    using gl::renderer_handler_window::renderer_handler_window;

    void on_key_pressed(gl::key pressed_key) override {
        using gl::key;

        switch (pressed_key) {
            case key::SPACE:
                ++ current_backend_index;
                if (current_backend_index >= backends.size())
                    current_backend_index = 0;

                set_backend_by_index();
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
        ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(475, 74));
        ImGui::Begin("Backend", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

        const char* current_name = backends[current_backend_index]->get_backend_name();

        if (ImGui::BeginCombo("Select Backend", current_name)) {
            for (int i = 0; i < backends.size(); ++ i) {
                const bool is_selected = (current_backend_index == i);

                if (ImGui::Selectable(backends[i]->get_backend_name(), is_selected)) {
                    current_backend_index = i;
                    set_backend_by_index();
                }

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }

        backends[current_backend_index]->additional_ui();

        ImGui::TextColored(ImVec4(1.0, 1.0, 0, 1), "FPS: %d", get_fps());

        ImGui::End();
    }

    void window_setup() override {
        add_backend<mandelbrot_gpu_single_precision_renderer>();
        add_backend<mandelbrot_gpu_renderer>();

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

void print_help(const char* executable_name) {
    std::cout << "Usage: " << executable_name                                   << "\n"
              << "   or: " << executable_name << " window_width window_height " << "\n"
                                                                                << "\n"
              << "==> Launch interactive mandelbrot set viewer"                 << std::endl;
}

int main(int argc, char* argv[]) {
    int width = 0, height = 0;

    const char* executable_name = argv[0];
    if (argc == 3) {
        // ==> Accepts CLI arguments in format:
        // ./<mandelbrot-executable> <width> <height>

        const char *width_string = argv[1],
            *height_string = argv[2];

        height = std::atoi(height_string);
        width  = std::atoi(width_string);
    } else if (argc != 1) {
        print_help(executable_name);
        return EXIT_FAILURE;
    }

    mandelbrot_window window(width, height, "Mandelbrot Set");
    window.draw_loop();
}
