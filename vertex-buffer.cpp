#include "vertex-buffer.h"

#include <GL/glew.h>

namespace gl {

    vertex_buffer::vertex_buffer(): data { NULL, 0 } {
        glGenBuffers(1, &id);
    }

    vertex_buffer::vertex_buffer(raw_data data_initializer)
        : data(data_initializer) {

        glGenBuffers(1, &id);

        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, data.size,
                     data.data, GL_STATIC_DRAW);
    }

    vertex_buffer::~vertex_buffer() {
        glDeleteBuffers(1, &id);
    }

    void vertex_buffer::set_data(raw_data new_data) {
        this->data = new_data;

        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, data.size,
                     data.data, GL_STATIC_DRAW);
    }

    size_t vertex_buffer::size() const {
        return data.size;
    }

    void vertex_buffer::bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, id);
    }

};
