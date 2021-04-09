#pragma once

#include "loops/md_index_loops.hpp"
#include "containers/structured_data.hpp"
#include "grid/neighbours.hpp"

namespace JADA{

template<size_t N>
struct Region{

    Region() = default;
    Region(position<N> begin, position<N> end, std::vector<direction<N>> deps) : 
    m_begin(begin), m_end(end), m_deps(deps) {}


    position<N> m_begin;
    position<N> m_end;
    std::vector<direction<N>> m_deps;

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
             [[maybe_unused]] Op   op,
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
void apply_stencil(StructuredData<N, T>& in, StructuredData<N, T>& out, [[maybe_unused]] Op op){

    //apply_interior(in, out, op);

    auto regions = create_regions(in, op);

    for (auto region : regions) {
        do_work(in, out, op, region);
    }



}



}