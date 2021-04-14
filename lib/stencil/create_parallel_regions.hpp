#pragma once

#include <vector>

#include "containers/region.hpp"
#include "loops/direction.hpp"

namespace JADA {

template <size_t N>
static dimension<N> parallel_region_dims(dimension<N> dims,
                                                   dimension<N> padding,
                                                   direction<N> dir) {

    dimension<N> h_dims{};

    for (size_t i = 0; i < N; ++i) {
        if (dir[i] != 0) {
            h_dims[i] = padding[i];
        } else {
            Utils::runtime_assert(dims[i] >= 2 * padding[i],
                                  "Negative halo dimension");
            h_dims[i] = dims[i] - 2 * padding[i];
        }
    }
    return h_dims;
}

template <size_t N>
static position<N> parallel_region_begin(dimension<N> dims,
                                                   dimension<N> padding,
                                                   direction<N> dir) {

    position<N> begin{};
    for (size_t i = 0; i < N; ++i) {
        if (dir[i] < 0) {
            begin[i] = 0;
        }

        else if (dir[i] == 0) {
            begin[i] = idx_t(padding[i]);
        }

        else {
            begin[i] = idx_t(dims[i] - padding[i]);
        }
    }
    return begin;
}

template <size_t N>
static position<N>
parallel_region_end(dimension<N> dims, dimension<N> padding, direction<N> dir) {

    return parallel_region_begin(dims, padding, dir) +
           position<N>(parallel_region_dims(dims, padding, dir));
}

template <size_t N, class Op>
static std::vector<Region<N>> create_parallel_regions(dimension<N> dims,
                                                      Op           op) {

    std::vector<Region<N>> ret;

    constexpr auto dirs    = Neighbours<N, ConnectivityType::Box>::get();
    const auto padding = compute_padding<N, Op>(op);

    // this is the interior region
    auto i_begin = position<N>(padding);
    auto i_end   = position<N>(dims) - position<N>(padding);

    ret.push_back(Region<N>(i_begin, i_end));

    for (direction<N> dir : dirs) {

        auto begin = parallel_region_begin(dims, padding, dir);
        auto end   = parallel_region_end(dims, padding, dir);

        ret.push_back(Region<N>(begin, end));
    }

    return ret;
}

} // namespace JADA