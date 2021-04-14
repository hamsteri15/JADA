#pragma once

#include <algorithm>
#include <execution>

#include "loops/md_index_loops.hpp"
#include "loops/md_range_indices.hpp"
#include "containers/structured_data.hpp"
#include "containers/md_view.hpp"
#include "containers/halo_offsets.hpp"
#include "grid/neighbours.hpp"
#include "communication/hpx_md_communicator.hpp"


namespace JADA{

template<size_t N>
struct Region{

    Region() = default;
    Region(position<N> begin, position<N> end, std::vector<direction<N>> deps) : 
    m_begin(begin), m_end(end), m_deps(deps) {}


    position<N> m_begin;
    position<N> m_end;
    std::vector<direction<N>> m_deps;


    position<N> begin() const {return m_begin;}
    position<N> end()   const {return m_end;}

};







template<size_t N, class T>
void fill_barrier(StructuredData<N, T>& in, direction<N> dir) {
    
    auto halo = in.get_halo(dir);
    halo.set_all(1);
    in.put_halo(halo, dir);

}


template <size_t N, class T, class Op>
void do_work(StructuredData<N, T>& in,
             StructuredData<N, T>& out,
             [[maybe_unused]] Op   op,
             position<N>           begin,
             position<N>           end) {

    
    for (auto pos : md_indices(begin, end)) { 
        out.at(pos) = op(pos, in); 
    }
}

template <size_t N, class T, class Op>
void do_work(StructuredData<N, T>& in,
             StructuredData<N, T>& out,
              Op   op,
             Region<N> region) {

    for (auto dir : region.m_deps){
        fill_barrier(in, dir);
    }


    do_work(in, out, op, region.m_begin, region.m_end);


}

template<size_t N, class Op>
dimension<N> compute_padding([[maybe_unused]] Op op) {

    dimension<N> padding;
    for (size_t i = 0; i < N; ++i){
        padding[i] = size_t(1);
    }
    return padding;
}


template<size_t N, class Op>
std::vector<Region<N>> create_regions(dimension<N> dims, Op op) {

    std::vector<Region<N>> ret;

    auto padding = compute_padding<N, Op>(op);

    //this is the interior region
    auto i_begin = position<N>(padding);
    auto i_end = position<N>(dims) - position<N>(padding);

    ret.push_back(
        Region<N>(i_begin, i_end, std::vector<direction<N>>{})
    );

    constexpr auto dirs = Neighbours<N, ConnectivityType::Box>::get();

    for (direction<N> dir : dirs) {
        if (dir.abs().elementwise_sum() < 2) {

            auto begin = interior_begin(dims, padding, dir);
            auto end = interior_end(dims, padding, dir);

            ret.push_back(Region<N>(begin, end, {dir}));          

        }
    }

    for (direction<N> dir : dirs) {
        if (dir.abs().elementwise_sum() >= 2) {

            auto begin = interior_begin(dims, padding, dir);
            auto end = interior_end(dims, padding, dir);

            ret.push_back(Region<N>(begin, end, {dir}));          

        }
    } 

    return ret;


}



template<size_t N, class Op, class T>
std::vector<Region<N>> create_regions(const StructuredData<N, T>& in, Op op) {

    return create_regions(in.get_dimension(), op);
}



template<size_t N, class T, class Op>
void apply_stencil(StructuredData<N, T>& in, StructuredData<N, T>& out,  Op op){

    //apply_interior(in, out, op);


    
    auto regions = create_regions(in, op);

    for (auto region : regions) {
        do_work(in, out, op, region);
    }



}



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


template <size_t N, class T, class Op>
void apply_stencil(const std::vector<T>&                in_v,
                   std::vector<T>&                      out_v,
                   dimension<N>                         dim,
                    Op                  op,
                   [[maybe_unused]] HpxMdCommunicator<std::vector<T>, N, ConnectivityType::Box> comm) {


    auto padding = compute_padding<N, Op>(op); 



    StructuredData<N, T> in(MdArray<N, T>(in_v, dim), dim, padding);
    StructuredData<N, T> out(dim, padding);



    call_sets(in_v, dim, op, comm);

    /*
    
    for (direction<N> dir : dirs) {

        if (comm.has_neighbour(dir)) {

            auto data = in.get_halo(dir);
            data.set_all(1);
            comm.set(dir, std::move(data.get_storage()), step);

        }
        else {
            //do something physical
        }


    }
    */
    std::cout << "tassa" << std::endl;


    size_t step = 0;
    auto dirs = comm.get_directions();
    for (direction<N> dir : dirs){

        if (comm.has_neighbour(dir)){
            auto data = comm.get(dir, step).get();

            auto b_dims = in.get_halo(dir).get_dimension();

            std::cout << data.size() << " " << b_dims << std::endl;

            Utils::runtime_assert(data.size() == b_dims.elementwise_product(), "Fail");


            in.put_halo(MdArray<N, T>(data, b_dims), dir);

        }

    }
    
    
    
    auto regions = create_regions(in, op);
    for (auto region : regions) {

        auto view = md_range_indices(region.begin(), region.end());
        std::for_each(
            std::execution::par,
            view.begin(), view.end(),
            [&](auto pos){
                out.at(pos) = op(pos, in);
            }
        );

    }




    const T* ptr = out.get_interior().data();

    for (size_t i = 0; i < dim.elementwise_product(); ++i){
        out_v[i] = ptr[i];
    }

}









}