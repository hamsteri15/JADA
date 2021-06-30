#pragma once

#include <vector>

#include "loops/dimension.hpp"

namespace JADA{


template<size_t N, class ArrIt, class Positions>
constexpr auto op_to_iterators(ArrIt in, Positions positions, dimension<N> dims) {


    std::vector<ArrIt> ret;

    auto shifts = get_shifts<N, StorageOrder::RowMajor>(dims);

    for (auto p : positions) {

        auto offset = std::inner_product(
            std::begin(p), std::end(p), std::begin(shifts), idx_t(0)
        );

        ret.push_back(in + offset);

    }
    return ret;

}
/*
template<size_t N, class It, class Op>
constexpr auto op_to_iterators(It in, dimension<N> dims, [[maybe_unused]] Op op) {


    auto p1 = position<2>{0, -1};
    auto p2 = position<2>{0,  1};
    auto p3 = position<2>{1,  0};
    auto p4 = position<2>{-1, 0};

    std::vector<position<N>> poss = {p1, p2, p3, p4};

    auto shifts = get_shifts<N, StorageOrder::RowMajor>(dims);


    std::vector<It> iters;

    for (auto p : poss) {

        auto offset = std::inner_product(
        std::begin(p), std::end(p), std::begin(shifts), idx_t(0));

        iters.push_back(in + offset);


    }

    return iters;

}
*/


}



