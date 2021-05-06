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
static Region<N>
create_parallel_region(dimension<N> dims, Op op, direction<N> dir) {

    const auto padding = compute_padding<N, Op>(op);

    return Region<N>(parallel_region_begin(dims, padding, dir),
                     parallel_region_end(dims, padding, dir));
    
}

template <size_t N, class Op>
static std::vector<Region<N>> create_parallel_regions(dimension<N> dims,
                                                      Op           op) {

    std::vector<Region<N>> ret(Neighbours<N, ConnectivityType::Box>::count());

    //create the interior region
    ret.push_back(create_parallel_region(dims, op, direction<N>{}));


    for (direction<N> dir : Neighbours<N, ConnectivityType::Box>::get()) {
        ret.emplace_back(create_parallel_region(dims, op, dir));
    }
    
    return ret;
}

//TODO: this doesnt belong here
//TODO: everything here should be constexpr
template<size_t N>
static std::vector<direction<N>> dependent_dirs(direction<N> dir) {

    // This should essentially divide the direction dir into basis vectors + the dir
    // such that
    // {1, 0}    becomes {1, 0} 
    // {1,1}     becomes {1, 0}, {0, 1} and {1,1}
    // {1, 0, 1} becomes {1, 0, 0}, {0, 0, 1}, {1, 0, 1} 
    // {1, -1, 1}becomes {1, 0, 0}, {0, -1, 0}, {0, 0, 1}, {1, -1, 1}

    //the operation may be called something like an orthonormal basis of dir

    if (dir.abs().elementwise_sum() == 0) {
        return {};
    }

    if (dir.abs().elementwise_sum() == 1) {
        return {dir};
    }

    std::vector<direction<N>> deps;

    deps.push_back(dir);

    for (size_t i = 0; i < N; ++i){

        if (dir[i] != 0){

            direction<N> dep{};
            dep[i] = dir[i];
            deps.push_back(dep);
        }


    }
    return deps;


}



} // namespace JADA