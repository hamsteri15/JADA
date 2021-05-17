#pragma once

#include <algorithm>
#include <execution>

#include "loops/md_index_loops.hpp"
#include "loops/md_range_indices.hpp"
#include "containers/structured_data.hpp"
#include "containers/structured_data_view.hpp"
#include "containers/md_view.hpp"
#include "grid/neighbours.hpp"
#include "communication/hpx_md_communicator.hpp"
#include "containers/region.hpp"
#include "stencil/compute_padding.hpp"
#include "stencil/create_interior_regions.hpp"
#include "stencil/create_parallel_regions.hpp"
#include "algorithms/get_slice.hpp"

namespace JADA{

template<size_t N, class T, class Op, ConnectivityType CT>
void call_set(const std::vector<T>& in, dimension<N> dim, Op op, HpxMdCommunicator<std::vector<T>, N, CT> comm, size_t step, direction<N> dir){

    auto padding = compute_padding<N, Op>(op);

    if (comm.has_neighbour(dir)) {
        auto r = create_interior_region(dim, padding, dir);
        auto slice = get_slice(in, dim, r.begin(), r.end());
        comm.set(dir, std::move(slice), step);
    }
}


template<size_t N, class T, class Op, ConnectivityType CT>
void call_sets(const std::vector<T>& in, dimension<N> dim, Op op, HpxMdCommunicator<std::vector<T>, N, CT> comm, size_t step){



    for (auto dir : comm.get_directions() ){
        call_set(in, dim, op, comm, step, dir);
    }

}



template <size_t N, class T, class Op, ConnectivityType CT>
void apply_stencil_boundaries( const std::vector<T>& in,
                    std::vector<T>&       out,
                    dimension<N>          dim,
                    Op                    op,
                    HpxMdCommunicator<std::vector<T>, N, CT> comm,
                    size_t step ) {




    

    Utils::runtime_assert(in.size() == out.size(),
                          "Invalid dimension to apply_stencil");
    Utils::runtime_assert(in.size() == dim.elementwise_product(), "Invalid dimension to apply_stencil.");

    auto padding = compute_padding<N, Op>(op);


    auto [datas, dirs] = comm.get_all_shared(step);

  
    MdView<N, std::vector<T>> s_out(dim, out);
    StructuredDataView<N, T, CT> s_in(dim, padding, in, datas, dirs);


    auto regions = create_parallel_regions(dim, op, false);

    
    //TODO: this should somehow account for physical boundaries as well
    for (auto r : regions) {

        auto idx_view = md_range_indices(r.begin(), r.end());
        std::for_each(
            std::execution::par,
            idx_view.begin(), idx_view.end(),
            [&](auto pos){
                s_out[pos] = op(pos, s_in);
            }
        );

    }

    call_sets(out, dim, op, comm, step + 1);

    
    
}



template <size_t N, class T, class Op, ConnectivityType CT>
void apply_stencil( const std::vector<T>& in,
                    std::vector<T>&       out,
                    dimension<N>          dim,
                    Op                    op,
                    HpxMdCommunicator<std::vector<T>, N, CT> comm,
                    size_t step ) {


    

    Utils::runtime_assert(in.size() == out.size(),
                          "Invalid dimension to apply_stencil");
    Utils::runtime_assert(in.size() == dim.elementwise_product(), "Invalid dimension to apply_stencil.");
    

    apply_stencil_boundaries(in, out, dim, op, comm, step);


    auto padding = compute_padding<N, Op>(op);
    StructuredData<N, T> s_in(MdArray<N, T>(in, dim), dim, padding);
    //MdView<N, std::vector<T>> s_in(dim, out);
    MdView<N, std::vector<T>> s_out(dim, out);


    auto r = create_parallel_region(dim, op, direction<N>{});


    auto idx_view = md_range_indices(r.begin(), r.end());
        std::for_each(
            std::execution::par,
            idx_view.begin(), idx_view.end(),
            [&](auto pos){
                s_out[pos] = op(pos, s_in);
            }
        );



}



}