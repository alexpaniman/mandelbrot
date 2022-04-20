#include "math.h" 
#include "vertex-layout.h"

#include <cassert>

namespace gl {
    template <>
    vertex_layout vertex::of_type<math::vec<double, 2>>(size_t count) {
        assert(count > 0);

        gl::vertex_layout layout = of_type<double>(2);
        for (size_t i = 0; i < count - 1; ++ i)
            layout = layout + of_type<double>(2);

        return layout;
    }
};
