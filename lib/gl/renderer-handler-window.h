#pragma once

#include "opengl-setup.h"
#include "renderer.h"

namespace gl {

    class renderer_handler_window: public gl::window {
    private:
        renderer* current_renderer = nullptr;

    public:
        using gl::window::window;

        void setup() override;
        void draw()  override;

        virtual void window_setup() {}

        void set_renderer(renderer* new_renderer);

        virtual ~renderer_handler_window() = default;
    };

};
