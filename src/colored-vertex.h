#pragma once

#include "math.h"

struct colored_vertex final {
    math::vec<double, 2> point;
    math::vec<double, 3> color;

    colored_vertex()
        : point(0.0, 0.0), color(0.0, 0.0, 0.0) {};

    colored_vertex(math::vec<double, 2> new_point,
                   math::vec<double, 3> new_color)
        : point(new_point), color(new_color) {};
};
