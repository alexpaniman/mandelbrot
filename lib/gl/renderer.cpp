#include "renderer.h"

namespace gl {

    renderer::renderer(): is_set_up(false) {}

    void renderer::setup_ensure_once(const size_t new_width, const size_t new_height) {
        if (!is_set_up) {
            this->width  = new_width;  // Setup renderer's dimensions 
            this->height = new_height;

            // Setup, just this once
            setup(); is_set_up = true;
        }
    }

    size_t renderer::get_width()  { return this->width;  }
    size_t renderer::get_height() { return this->height; }

};
