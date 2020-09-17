#pragma once

#include "stencil/stencil_indices.hpp"
#include "utils/constexpr_functions.hpp"

namespace JADA {

template <class Derived> struct StencilOperation {

    static constexpr auto get_indices() {
        return Derived::indices;
    }

    static constexpr idx_t left_halfwidth() {

        using namespace Utils;
        return constexpr_abs(min(Derived::indices));
    }

    static constexpr idx_t right_halfwidth() {

        using namespace Utils;
        return constexpr_abs(max(Derived::indices));
    }

    static constexpr idx_t required_barriers() {

        using namespace Utils;
        return constexpr_max(left_halfwidth(), right_halfwidth());        
    }

    template <class... Ts> static auto apply(const Ts&... values) {
        Derived::apply(values...);
    }
};

} // namespace JADA