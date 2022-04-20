#pragma once

#include "vertex-buffer.h"
#include "vertex-layout.h"

#include <GL/glew.h>

#include <initializer_list>
#include <vector>

namespace gl {

    class vertex_array final {
    private:
        unsigned int id;

        vertex_buffer buffer;
        vertex_layout layout;

    public:
        vertex_array();
        vertex_array(size_t size);
        vertex_array(vertex_buffer buffer_initializer, vertex_layout layout_initalizer);

        void assign_layout(vertex_layout layout_initializer);
        void assign_buffer(vertex_buffer buffer_initializer);
        void assign(vertex_buffer buffer_initializer, vertex_layout layout_initializer);
        void assing();

        size_t size() const;

        void bind() const;

        ~vertex_array();
    };

};
