#pragma once


#include <vector>

#include "containers/region.hpp"
#include "loops/direction.hpp"

namespace JADA {

template <size_t N>
static constexpr dimension<N> interior_region_dims(dimension<N> dims,
                                                   dimension<N> padding,
                                                   direction<N> dir) {
    dimension<N> i_dims{};

    for (size_t i = 0; i < N; ++i) {
        if (dir[i] != 0) {
            i_dims[i] = padding[i];
        } else {
            i_dims[i] = dims[i];
        }
    }

    return i_dims;
}

template <size_t N>
static constexpr position<N> interior_region_begin(dimension<N> dims,
                                                    dimension<N> padding,
                                                    direction<N> dir) {

    position<N> begin{};

    for (size_t i = 0; i < N; ++i) {
        if (dir[i] > 0) { begin[i] = idx_t(dims[i] - padding[i]); }
    }
    return begin;
}

template <size_t N>
static constexpr position<N>
interior_region_end(dimension<N> dims, dimension<N> padding, direction<N> dir) {

    return interior_region_begin(dims, padding, dir) +
           position<N>(interior_region_dims(dims, padding, dir));
}



template<size_t N>
static constexpr Region<N>
create_interior_region(dimension<N> dims, dimension<N> padding, direction<N> dir) {

    const auto begin = interior_region_begin(dims, padding, dir);
    const auto end = interior_region_end(dims, padding, dir);

    return Region<N>(begin, end, dir);


}
/*
template<size_t N, class Op>
static constexpr std::vector<Region<N>> create_interior_regions(dimension<N> dims, dimension<N> padding, Op op){

    std::vector<Region<N>> ret;

    constexpr auto dirs    = Neighbours<N, ConnectivityType::Box>::get();
    constexpr auto padding = compute_padding<N, Op>(op);

    // this is the interior region
    auto i_begin = position<N>(padding);
    auto i_end   = position<N>(dims) - position<N>(padding);

    ret.push_back(Region<N>(i_begin, i_end, std::vector<direction<N>>{}));

    for (direction<N> dir : dirs) {

        auto begin = interior_begin(dims, padding, dir);
        auto end   = interior_end(dims, padding, dir);

        ret.push_back(Region<N>(begin, end, {dir}));
    }

    return ret;

}
*/


} // namespace JADA
