#pragma once

#include "loops/dimension.hpp"

namespace JADA{

template <size_t N, class Op>
constexpr dimension<N> compute_padding([[maybe_unused]] Op op) {

    dimension<N> padding;
    for (size_t i = 0; i < N; ++i) { padding[i] = size_t(1); }
    return padding;
}


}


