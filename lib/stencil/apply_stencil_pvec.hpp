#pragma once

#include <cstddef>
#include <hpx/algorithm.hpp>
#include <hpx/include/partitioned_vector.hpp>
#include <hpx/runtime_distributed/find_localities.hpp>

#include "loops/dimension.hpp"
#include "containers/partitioned_vector.hpp"
#include "loops/flatten_index.hpp"

namespace JADA{

template<size_t N, class T>
static bool valid_pvec(hpx::partitioned_vector<T> in, dimension<N> dims) {

    return (in.size() == dims.elementwise_product()) && uniform_segments(in);    

}


template<size_t N, class It, class Op>
std::vector<It> get_neighbours(It in, dimension<N> dims, [[maybe_unused]] Op op) {


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






template<size_t N, class InputIt, class OutputIt, class Op>
void do_work_segment(InputIt begin, InputIt end, OutputIt out, dimension<N> dims, [[maybe_unused]] Op op){

    using ET = typename std::iterator_traits<InputIt>::value_type;

    auto neigh = get_neighbours(begin, dims, op);


    for (auto it = begin; it != end; ++it) {

        ET ret(0);

        for (auto n : neigh) {
            ret += *n;
            ++n;
        }
        *out = ret;
        ++out;

         


    }




}


template<size_t N, class T, class Op>
hpx::partitioned_vector<T> apply_stencil(hpx::partitioned_vector<T> in, dimension<N> dims, Op op){

    Utils::runtime_assert(valid_pvec(in, dims));

    //TODO: ensure non-blocking
    hpx::partitioned_vector<T> ret = make_partitioned_vector<N, T>(dims);



    //The code here should be pretty much identical to a std::vector<T>:
    // for all segments:

    // 1) compute pos_0 = begin + some_offset handling the stencil width to physical boundary
    // 2) compute offsets to pos_0 based on the op (can be precomputed since they are same for all segments)
    // 3) convert offsets to iterators
    // 4) pass to op 





    return ret;



}




    
}

