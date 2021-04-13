#pragma once

#include <algorithm>
#include <execution>

#include "loops/md_index_loops.hpp"
#include "loops/md_range_indices.hpp"
#include "containers/structured_data.hpp"
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
position<N> get_begin(const StructuredData<N, T>& in, direction<N> dir){

    auto dims = in.get_dimension();
    auto padding = in.get_padding();

    position<N> begin{};

    for (size_t i = 0; i < N; ++i){
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

template <size_t N, class T>
position<N> get_end(const StructuredData<N, T>& in, direction<N> dir) {

    auto         dims    = in.get_dimension();
    auto         padding = in.get_padding();
    
    dimension<N> h_dims{};

    for (size_t i = 0; i < N; ++i) {
        if (dir[i] != 0) {
            h_dims[i] = padding[i];
        } 
        else {
            h_dims[i] = dims[i] - 2*padding[i];
        }
    }

    return get_begin(in, dir) + position<N>(h_dims);

    
}




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




template<size_t N, class Op, class T>
std::vector<Region<N>> create_regions(const StructuredData<N, T>& in, [[maybe_unused]] Op op) {

    std::vector<Region<N>> ret;


    auto i_begin = in.begin() + position<N>(in.get_padding());
    auto i_end = in.end() - position<N>(in.get_padding());

    ret.push_back(
        Region<N>(i_begin, i_end, std::vector<direction<N>>{})
    );

    constexpr auto dirs = Neighbours<N, ConnectivityType::Box>::get();

    for (direction<N> dir : dirs) {
        if (dir.abs().elementwise_sum() < 2) {

            auto begin = get_begin(in, dir);
            auto end = get_end(in, dir);

            ret.push_back(Region<N>(begin, end, {dir}));          

        }
    }

    for (direction<N> dir : dirs) {
        if (dir.abs().elementwise_sum() >= 2) {

            auto begin = get_begin(in, dir);
            auto end = get_end(in, dir);

            ret.push_back(Region<N>(begin, end, {dir}));          

        }
    } 

    return ret;


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
void apply_stencil(StructuredData<N, T>& in, StructuredData<N, T>& out,  Op op,  HpxMdCommunicator<std::vector<T>, N, ConnectivityType::Box> comm){

    //apply_interior(in, out, op);


    auto dirs = Neighbours<N, ConnectivityType::Box>::get();

    size_t step = 0;

    for (direction<N> dir : dirs) {

        if (comm.has_neighbour(dir)) {
            
            std::cout << dir << std::endl;
            auto temp = in.get_halo(dir);
            temp.set_all(1);
            //auto tt = temp.get_storage();
            comm.set(dir, std::move(temp.get_storage()), step);
        }
    }
    
    //hpx::barrier();

    //this deadlocks because not all comms have called set yet, only the input comm.
    // it seems that the comm has to be tied to the data structure :/
    /*
    for (direction<N> dir : dirs) {

        if (comm.has_neighbour(dir)) {
            
            auto n_data = comm.get(dir, step).get();
            MdArray<N, T> n_data_structured(n_data, in.get_halo(dir).get_dimension());
            in.put_halo(n_data_structured, dir);
        }
    }
    */
    
    auto regions = create_regions(in, op);
    for (auto region : regions) {

        do_work(in, out, op, region.begin(), region.end());

    }




    /*
    size_t step = 0;
    
    auto n_data = comm.get(dir, step).get();
    MdArray<N, T> n_data_structured(n_data, in.get_halo(dir).get_dimension());
    in.put_halo(n_data_structured, dir);
    
    auto temp = in.get_halo(dir); //should be get interior and done after solved
    temp.set_all(1);
    auto tt = temp.get_storage();
    comm.set(dir, std::move(tt), step);

    for (auto region : regions) {
        do_work(in, out, op, region, comm);
    }

    */

}


template<size_t N, class T, class Op>
void apply_stencil(const std::vector<T>& in_v, std::vector<T>& out_v, dimension<N> dim, [[maybe_unused]] Op op) {

    dimension<N> padding;
    for (size_t i = 0; i < N; ++i){
        padding[i] = size_t(1);
    }



    StructuredData<N, T> in(MdArray<N, T>(in_v, dim), dim, padding);
    StructuredData<N, T> out(dim, padding);


    apply_stencil(in, out, op);

    const T* ptr = out.get_interior().data();

    for (size_t i = 0; i < dim.elementwise_product(); ++i){
        out_v[i] = ptr[i];
    }

}

template <size_t N, class T, class Op>
void apply_stencil(const std::vector<T>&                in_v,
                   std::vector<T>&                      out_v,
                   dimension<N>                         dim,
                    Op                  op,
                   [[maybe_unused]] HpxMdCommunicator<std::vector<T>, N, ConnectivityType::Box> comm) {


    dimension<N> padding;
    for (size_t i = 0; i < N; ++i){
        padding[i] = size_t(1);
    }



    StructuredData<N, T> in(MdArray<N, T>(in_v, dim), dim, padding);
    StructuredData<N, T> out(dim, padding);


//    apply_stencil(in, out, op, comm);


    auto interior_region = create_regions(in, op).front();
    auto view = md_range_indices(interior_region.begin(), interior_region.end());

    std::for_each(
        std::execution::par,
        view.begin(), view.end(),
        [&](auto pos) {
            auto [i, j] = pos;
            position<2> ppos = {i, j};
            out.at(ppos) = op(ppos, in);
            //std::cout << pos[0] << std::endl;
            //position<N> ppos = pos;
            //out.at(pos) = op(pos, in); 
        }
    );

    






    const T* ptr = out.get_interior().data();

    for (size_t i = 0; i < dim.elementwise_product(); ++i){
        out_v[i] = ptr[i];
    }

}









}