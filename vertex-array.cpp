#include "vertex-array.h"
#include "vertex-buffer.h"
#include "vertex-layout.h"

namespace gl {
    vertex_array::vertex_array(): buffer(), layout() {
        glGenVertexArrays(1, &this->id);
    }

    vertex_array::vertex_array(vertex_buffer new_buffer, vertex_layout new_layout)
        : buffer(new_buffer), layout(new_layout) {

        glGenVertexArrays(1, &this->id);
        assign(new_buffer, new_layout);
    }

    void vertex_array::assign_layout(vertex_layout layout_initializer) {
        this->layout = layout_initializer;

        this->bind();
        this->buffer.bind();

        unsigned int offset = 0;
        for (unsigned int i = 0; i < this->layout.vertices.size(); ++ i) {
            const vertex& layout_element = this->layout.vertices[i];

            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, layout_element.count, layout_element.type_id,
                                  GL_FALSE, layout_element.size,
                                  (const void*)(uintptr_t) offset);

            offset += layout_element.count * layout_element.size;
        }
    }

    void vertex_array::assign_buffer(vertex_buffer new_buffer) {
        this->buffer = new_buffer;
    }

    void vertex_array::assign(vertex_buffer new_buffer, vertex_layout new_layout) {
        assign_buffer(new_buffer);
        assign_layout(new_layout);
    }

    size_t vertex_array::size() const {
        return buffer.size();
    }      

    vertex_array::~vertex_array() {}

    void vertex_array::bind() const { glBindVertexArray(this->id); }

};
