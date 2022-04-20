#pragma once

#include <cstddef>

namespace math {

    template <typename element_type, size_t count>
    class vec {
    protected:
        element_type coordinates[count];

    public:
        template<class... vector_coordinates>
        vec(vector_coordinates... initializer_coordinates)
            : coordinates { initializer_coordinates... } {

            const int n = sizeof...(vector_coordinates);
            static_assert(n == count, "Invalid number of vector coordinates!");
        }


        #define COORDINATE_GETTER(coordinate_name, index)                  \
            constexpr element_type &coordinate_name() {                    \
                static_assert(count > index, "Vector doesn't have `"       \
                                             #coordinate_name              \
                                             "' because it's too small!"); \
                return this->coordinates[index];                           \
            }                                                              \

        #define COORDINATE_GETTERS(name0, name1, name2, name3)             \
            COORDINATE_GETTER(name0, 0)                                    \
            COORDINATE_GETTER(name1, 1)                                    \
            COORDINATE_GETTER(name2, 2)                                    \
            COORDINATE_GETTER(name3, 3)                                    \

        // Define same coordinate getters that OpenGL uses 
        COORDINATE_GETTERS(x, y, z, w)
        COORDINATE_GETTERS(r, g, b, a)
        COORDINATE_GETTERS(s, t, p, q)


        element_type& operator[](const size_t index) {
            return this->coordinates[index];
        }
    };

    using dvec2 = vec<double, 2>;
    using dvec3 = vec<double, 3>;
    using dvec4 = vec<double, 4>;
}
