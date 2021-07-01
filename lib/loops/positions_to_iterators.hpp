#pragma once

#include <vector>

#include "loops/dimension.hpp"
#include "loops/flatten_index.hpp"
#include "loops/iterator_set.hpp"
#include "loops/position_set.hpp"

namespace JADA {

namespace detail {

template <size_t N, class It>
static constexpr It
position_to_iterator(position<N> pos, It center, dimension<N> shifts) {

    auto offset = std::inner_product(
        std::begin(pos), std::end(pos), std::begin(shifts), idx_t(0));
    return center + offset;
}

} // namespace detail

template <size_t N, class It>
static constexpr dynamic_iterator_set<It>
positions_to_iterators(PositionSet<N> positions, It center, dimension<N> dims) {

    auto shifts = get_shifts<N, StorageOrder::RowMajor>(dims);

    std::vector<It> iters;
    iters.reserve(positions.size());

    for (auto pos : positions) {

        iters.emplace_back(detail::position_to_iterator(pos, center, shifts));
    }

    return dynamic_iterator_set<It>(iters);
}

} // namespace JADA