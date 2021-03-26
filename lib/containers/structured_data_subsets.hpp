#pragma once

#include "loops/dimension.hpp"
#include "loops/position.hpp"

namespace JADA {

template <size_t N>
position<N> halo_extent(dimension<N> dims,
                        position<N>  dir,
                        dimension<N> padding = dimension<N>{}) {

    position<N> extent{};

    for (size_t i = 0; i < N; ++i) {
        if (std::abs(dir[i]) > 0) {
            extent[i] = idx_t(padding[i]);
        } else {
            extent[i] = idx_t(dims[i]);
        }
    }

    return extent;
}

template <size_t N>
position<N> halo_begin(dimension<N> dims,
                       position<N>  dir,
                       dimension<N> padding = dimension<N>{}) {

    position<N> begin{};

    for (size_t i = 0; i < N; ++i) {

        if (dir[i] < 0) {
            begin[i] = 0;
        } else if (dir[i] > 0) {
            begin[i] = idx_t(dims[i] + padding[i]);
        } else if (dir[i] == 0) {
            begin[i] = idx_t(padding[i]);
        }
    }
    return begin;
}

template <size_t N>
position<N> halo_end(dimension<N> dims,
                     position<N>  dir,
                     dimension<N> padding = dimension<N>{}) {
    return halo_begin(dims, dir, padding) + halo_extent(dims, dir, padding);
}

template <size_t N>
position<N> boundary_begin(dimension<N> dims,
                           position<N>  dir,
                           dimension<N> padding = dimension<N>{}) {

    auto begin = halo_begin(dims, dir, padding);
    begin -= dir * position<N>(padding);
    return begin;
}

template <size_t N>
position<N> boundary_end(dimension<N> dims,
                         position<N>  dir,
                         dimension<N> padding = dimension<N>{}) {

    return boundary_begin(dims, dir, padding) + halo_extent(dims, dir, padding);
}

} // namespace JADA