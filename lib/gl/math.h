#pragma once

#include <cstddef>
#include <cassert>
#include <ostream>
#include <tuple>

#include "vertex-layout.h"

namespace math {

    template <typename element_type, size_t count>
    class vec {
    private:
        element_type coordinates[count];

    public:
        template<class... vector_coordinates>
        constexpr vec(vector_coordinates... initializer_coordinates)
            : coordinates { initializer_coordinates... } {

            const int n = sizeof...(vector_coordinates);
            static_assert(n == count, "Invalid number of vector coordinates!");
        }


        #define COORDINATE_GETTER(coordinate_name, index)                                                    \
            constexpr element_type &coordinate_name() {                                                      \
                static_assert(count > index, "Vector doesn't have `" #coordinate_name                        \
                                             "' because it's too small!");                                   \
                                                                                                             \
                return this->coordinates[index];                                                             \
            }                                                                                                \

        #define COORDINATE_GETTERS(name0, name1, name2, name3)                                               \
            COORDINATE_GETTER(name0, 0) COORDINATE_GETTER(name1, 1)                                          \
            COORDINATE_GETTER(name2, 2) COORDINATE_GETTER(name3, 3)                                          \

        // Define same coordinate getters that OpenGL uses 
        COORDINATE_GETTERS(x, y, z, w) COORDINATE_GETTERS(r, g, b, a) COORDINATE_GETTERS(s, t, p, q)


        constexpr       element_type& operator[](const size_t index)       {
            return this->coordinates[index];
        }

        constexpr const element_type& operator[](const size_t index) const {
            return this->coordinates[index];
        }


        constexpr       element_type* begin()       { return coordinates;         }
        constexpr const element_type* begin() const { return coordinates;         }

        constexpr       element_type*   end()       { return coordinates + count; }
        constexpr const element_type*   end() const { return coordinates + count; }


        #define DEFINE_ASSIGNMENT(assignment)                                                                \
            constexpr vec<element_type, count>& operator assignment(const vec<element_type, count>& other) { \
                for (int i = 0; i < count; ++ i)                                                             \
                    (*this)[i] assignment other[i];                                                          \
                                                                                                             \
                return *this;                                                                                \
            }

        DEFINE_ASSIGNMENT(*=) DEFINE_ASSIGNMENT(/=) DEFINE_ASSIGNMENT(-=) DEFINE_ASSIGNMENT(+=)

        #undef DEFINE_ASSIGNMENT


        #define DEFINE_OPERATOR(name, corresponding_assignment)                                              \
            constexpr vec<element_type, count> operator name(const vec<element_type, count>& other) {        \
                vec<element_type, count> new_vector = *this;                                                 \
                return new_vector corresponding_assignment other;                                            \
            }                                                                                                \

        DEFINE_OPERATOR(*, *=) DEFINE_OPERATOR(-, -=) DEFINE_OPERATOR(+, +=) DEFINE_OPERATOR(/, /=)

        #undef DEFINE_OPERATOR


        constexpr vec<element_type, count>& operator*=(const element_type value) {
            for (element_type& coordinate: coordinates)
                coordinate *= value;

            return *this;
        }

        constexpr vec<element_type, count>  operator* (const element_type value) {
            vec<element_type, count> new_vector = *this;
            return new_vector *= value;
        }


        constexpr element_type dot(const vec<element_type, count>& other) {
            element_type accumulator {};
            for (int i = 0; i < count; ++ i)
                accumulator += (*this)[i] * other[i];

            return accumulator;
        }

        constexpr element_type len() { return this->dot(*this); }


        template <typename value_type, size_t element_count>
        friend std::ostream& operator<<(std::ostream& os, math::vec<value_type, element_count> vector);

    };

    template <typename element_type, size_t count>
    static gl::vertex_layout vector_layout(const size_t number_of_vectors = 1) {
        assert(number_of_vectors > 0);

        gl::vertex_layout layout = gl::vertex::of_type<element_type>(count);
        for (size_t i = 0; i < number_of_vectors - 1; ++ i)
            layout = layout + gl::vertex::of_type<element_type>(count);

        return layout;
    }

    template <typename value_type, size_t count>
    constexpr vec<value_type, count> operator*(const value_type value,
                                               const vec<value_type, count> vec) {

        return vec * value;
    }

    template <typename value_type, size_t count>
    std::ostream& operator<<(std::ostream& os, const math::vec<value_type, count> vector) {
        os << "(";

        for (int i = 0; i < count; ++ i) {
            os << vector[i];
            if (i != count - 1) os << ", ";
        }

        return os << ")";
    }

    template<class... vector_coordinates>
    vec(vector_coordinates... initializer_coordinates) ->
        vec<typename std::tuple_element<0, std::tuple<vector_coordinates...>>::type, sizeof...(vector_coordinates)>;

    template <typename type>
    constexpr type pow(type value, int power) {
        if (power == 1)
            return value;

        return power % 2 == 0?
              pow(value * value, power / 2)
            : value * pow(value, power - 1);
    }

}
