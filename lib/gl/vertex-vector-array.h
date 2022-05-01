#pragma once

#include "vertex-array.h"
#include "vertex-buffer.h"
#include "vertex-layout.h"

#include <vector>

namespace gl {

    template <typename value_type>
    class vertex_vector_array {
    private:
        std::vector<value_type> elements;
        vertex_array element_array_holder;

    public:
        vertex_vector_array(): elements(), element_array_holder() {}

        void set_layout(gl::vertex_layout layout) {
            element_array_holder.set_layout(layout);
        }

        void update() { element_array_holder.assign(elements); }

        const vertex_array& get_vertex_array() const {
            return element_array_holder;
        }

        constexpr void assign(std::initializer_list<value_type> init) {
            elements.assign(init);
        }

        void assign_and_update(std::initializer_list<value_type> init) {
            elements.assign(init);
            update();
        }

        void resize(size_t new_size, const value_type& value = value_type()) {
            elements.resize(new_size, value);
        }

        constexpr auto& operator[](const size_t index) {
            return elements[index];
        }

        constexpr auto begin() { return elements.begin(); }
        constexpr auto end()   { return elements.end();   }
    };

};
