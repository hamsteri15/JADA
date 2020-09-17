#pragma once

#include "operation/stencil_operation.hpp"
#include "stencil/stencil_indices.hpp"

namespace JADA {

// Second order central-difference for the second derivative
struct DDcd2 : public StencilOperation<DDcd2> {

    static constexpr StencilIndices<3> indices = {-1, 0, 1};

    template <class T>
    static T apply(const T& v1, const T& v2, const T& v3) {
        return T(v1 - 2.0 * v2 + v3);
    }
};

// Fourth order central-difference for the second derivative
struct DDcd4 : public StencilOperation<DDcd4> {

    static constexpr StencilIndices<5> indices = {-2, -1, 0, 1, 2};

    template <class T>
    static T
    apply(const T& v1, const T& v2, const T& v3, const T& v4, const T& v5) {
        return T((-v1 + 16.0 * v2 - 30.0 * v3 + 16.0 * v4 - v5) / 12.0);
    }
};
} // namespace JADA
