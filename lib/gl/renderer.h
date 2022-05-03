#pragma once

#include <cstddef>

namespace gl {

    class renderer {
    private:
        size_t width, height;
        bool is_set_up;

    public:
        renderer();

        virtual void setup() {};
        virtual void draw() = 0;

        size_t get_width();
        size_t get_height();

        void setup_ensure_once(size_t new_width, size_t new_height);

        virtual ~renderer() = default;
    };

};
