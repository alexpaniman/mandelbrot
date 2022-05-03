#include "renderer-handler-window.h"
#include "renderer.h"

namespace gl {

    void renderer_handler_window::setup() {
        window_setup();
        current_renderer->setup_ensure_once(this->width, this->height);
    }

    void renderer_handler_window::draw()  { current_renderer->draw(); }

    void renderer_handler_window::set_renderer(renderer* new_renderer) {
        new_renderer->setup_ensure_once(this->width, this->height);
        this->current_renderer = new_renderer;
    }

};
