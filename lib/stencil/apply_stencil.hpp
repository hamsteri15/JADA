#pragma once

#include <algorithm>
#include <execution>

#include "loops/md_index_loops.hpp"
#include "loops/md_range_indices.hpp"
#include "containers/structured_data.hpp"
#include "containers/md_view.hpp"
#include "grid/neighbours.hpp"
#include "communication/hpx_md_communicator.hpp"
#include "containers/region.hpp"
#include "stencil/compute_padding.hpp"
#include "stencil/create_interior_regions.hpp"
#include "stencil/create_parallel_regions.hpp"


namespace JADA{

template <size_t N, class T, class Op>
void apply_stencil([[maybe_unused]] const std::vector<T>& in,
                   [[maybe_unused]] std::vector<T>&       out,
                   [[maybe_unused]] dimension<N>          dim,
                   [[maybe_unused]] Op                    op,
                   [[maybe_unused]] HpxMdCommunicator<std::vector<T>, N, ConnectivityType::Box> comm ) {

    

    Utils::runtime_assert(in.size() == out.size(),
                          "Invalid dimension to apply_stencil");
    Utils::runtime_assert(in.size() == dim.elementwise_product(), "Invalid dimension to apply_stencil.");


    auto padding = compute_padding<N, Op>(op);


    StructuredData<N, T> s_in(MdArray<N, T>(in, dim), dim, padding);
    StructuredData<N, T> s_out(dim, padding);

    size_t step = 0;

    auto dirs = Neighbours<N, ConnectivityType::Box>::get();

    for (direction<N> dir : dirs ){

        if (comm.has_neighbour(dir)) {
            auto r = create_interior_region(dim, padding, dir);
            std::vector<T> slice(r.get_dimension().elementwise_product());
            size_t i = 0;
            for (auto pos : md_indices(r.begin(), r.end())){

                slice[i] = s_in.at(pos);
                ++i;
            }

            comm.set(dir, std::move(slice), step);
        }

    }



    
    for (direction<N> dir : dirs) {

        if (comm.has_neighbour(dir)){

            std::vector<T> data = comm.get(dir, step).get();
            auto slice_dims = create_interior_region(dim, padding, dir).get_dimension();
            s_in.put_halo(MdArray<N, T>(data, slice_dims), dir);

        }

    }


    for (auto r : create_parallel_regions(dim, op)) {

        auto idx_view = md_range_indices(r.begin(), r.end());
        std::for_each(
            std::execution::par,
            idx_view.begin(), idx_view.end(),
            [&](auto pos){
                s_out.at(pos) = op(pos, s_in);
            }
        );

    }


    out = s_out.get_interior().get_storage();


}

/*
template<size_t N, class T, class Op>
void apply_stencil(const std::vector<T>& in_v, std::vector<T>& out_v, dimension<N> dim, [[maybe_unused]] Op op) {

    auto padding = compute_padding<N, Op>(op);


    StructuredData<N, T> in(MdArray<N, T>(in_v, dim), dim, padding);
    StructuredData<N, T> out(dim, padding);


    apply_stencil(in, out, op);

    const T* ptr = out.get_interior().data();

    for (size_t i = 0; i < dim.elementwise_product(); ++i){
        out_v[i] = ptr[i];
    }

}
*/


/*
template <size_t N, class T, class Op>
void call_sets(
    const std::vector<T>&                                       in,
    dimension<N>                                                dim,
    [[maybe_unused]] Op                                                          op,
    HpxMdCommunicator<std::vector<T>, N, ConnectivityType::Box> comm) {

    size_t step  =0;

    auto dirs = comm.get_directions();

    auto padding = compute_padding<N, Op>(op);

    auto view = MdView(dim, in);

    for (direction<N> dir : dirs) {

        if (comm.has_neighbour(dir)) {

            auto begin = interior_begin(dim, padding, dir);
            auto end = interior_end(dim, padding, dir);

            std::vector<T> data;
            for (auto pos : md_range_indices(begin, end)){
                data.push_back(view[pos]);
            }
            comm.set(dir, std::move(data), step);
            

        }

    }


}
*/














}