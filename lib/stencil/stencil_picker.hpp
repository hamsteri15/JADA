#pragma once

#include <functional>
#include <tuple>

#include "loops/index_type.hpp"
#include "stencil/stencil_indices.hpp"

namespace JADA {

namespace detail {

template <typename Array, size_t... Is>
static constexpr auto pick_stencil(const Array&                         a,
                                   const StencilIndices<sizeof...(Is)>& indices,
                                   idx_t current_idx,
                                   idx_t offset,
                                   const std::index_sequence<Is...>&) {
    // TODO: Add a debug bounds check
    return std::make_tuple(
        a[current_idx + size_t(indices[Is] * int(offset))]...);
}
} // namespace detail

struct StencilPicker {

    template <typename Array, size_t N>
    static constexpr auto pick_stencil(const Array&             a,
                                       const StencilIndices<N>& indices,
                                       idx_t                    current_idx,
                                       idx_t                    offset) {

        return detail::pick_stencil(
            a, indices, current_idx, offset, std::make_index_sequence<N>{});
    }
};

} // namespace JADA