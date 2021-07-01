#pragma once

#include <cstddef>
#include <hpx/algorithm.hpp>
#include <hpx/include/partitioned_vector.hpp>
#include <hpx/runtime_distributed/find_localities.hpp>

#include "loops/dimension.hpp"
#include "containers/partitioned_vector.hpp"
#include "loops/flatten_index.hpp"
#include "loops/positions_to_iterators.hpp"

namespace JADA{

template<size_t N, class T>
static bool valid_pvec(hpx::partitioned_vector<T> in, dimension<N> dims) {

    return (in.size() == dims.elementwise_product()) && uniform_segments(in);    

}


template<size_t N, class It, class Op>
auto get_stencil(It in, dimension<N> dims, [[maybe_unused]] Op op) {

    std::vector<position<N>> points(op.stencil.begin(), op.stencil.end());
    PositionSet<N> poss(points);

    return positions_to_iterators(poss, in, dims);

}






template<size_t N, class InputIt, class OutputIt, class Op>
void do_work_segment(InputIt begin, InputIt end, OutputIt out, dimension<N> dims, [[maybe_unused]] Op op){


    auto stencil = get_stencil(begin, dims, op);


    for (auto it = begin; it != end; ++it) {

        *out = op(stencil);
        ++out;

    }




}

template<class Op>
auto find_min_max(Op op){

    auto center = op.center;
    auto stencil = op.stencil;

    auto min = center;
    auto max = center;

    for (size_t i = 0; i < center.size(); ++i){

        for (auto n : stencil){

            min[i] = std::min(n[i], min[i]);
            max[i] = std::max(n[i], max[i]);


        }

    }
    
    return std::make_pair(min, max);


}


template<size_t N, class Op>
auto segment_loop_bounds(dimension<N> dims, Op op){

    position<N> begin{};
    
    auto [min, max] = find_min_max(op);

    for (size_t i = 0; i < N; ++i){

        if (min[i] < 0){
            begin[i] = std::abs(min[i]);
        }
        else {
            begin[i] = 0;
        }
    }

    position<N> end = begin;

    for (size_t i = 0; i < size_t(N - 1); ++i){
        
        if (max[i] > 0) {
            end[i] = idx_t(dims[i]) - max[i];
        }
        else {
            end[i] = idx_t(dims[i]);
        }        

    }

    end.back() += 1;

    return std::make_pair(begin, end);



}


template<size_t N, class Iterable, class Op>
void apply_stencil(const Iterable& in, Iterable& out, dimension<N> dims, Op op){

    auto [begin, end] = segment_loop_bounds(dims, op);


    for (auto pos : md_indices(begin, end)){

    
        auto idx = flatten<N, StorageOrder::RowMajor>(dims, pos);
        auto segment_begin = in.begin() + idx;
        auto segment_end = segment_begin + idx_t(dims.back() - 2);
        auto segment_out_begin = out.begin() + idx;

        do_work_segment(segment_begin, segment_end, segment_out_begin, dims, op);

    }





}


template<size_t N, class T, class Op>
std::vector<T> apply_stencil(std::vector<T> in, dimension<N> dims, Op op){

    Utils::runtime_assert(in.size() == dims.elementwise_product(), "Dimension mismatch in apply stencil.");

    std::vector<T> out(in.size());

    apply_stencil(in, out, dims, op);


    return out;


}

template<size_t N, class T, class Op>
hpx::partitioned_vector<T> apply_stencil(hpx::partitioned_vector<T> in, dimension<N> dims, Op op){

    Utils::runtime_assert(valid_pvec(in, dims), "Invalid partitioned vector");

    //TODO: ensure non-blocking
    hpx::partitioned_vector<T> out = make_partitioned_vector<N, T>(dims);

    apply_stencil(in, out, dims, op);

  
    return out;


}




    
}

