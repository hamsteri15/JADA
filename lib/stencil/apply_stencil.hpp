#pragma once

#include "loops/md_index_loops.hpp"
#include "containers/structured_data.hpp"
#include "grid/neighbours.hpp"

namespace JADA{



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

template<size_t N, class T>
void fill_barriers(StructuredData<N, T>& in) {

    Neighbours<N, ConnectivityType::Star> n;    

    for (direction<N> dir : n.get()) {

        fill_barrier(in, dir);

    }
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

template<size_t N, class T, class Op>
void apply_interior(StructuredData<N, T>& in, StructuredData<N, T>& out, [[maybe_unused]] Op op){
    
    //auto begin = get_begin(in, direction<N>{});
    //auto end = in.end() - 1; 
    //auto end = get_end(in, direction<N>{});

    auto begin = in.begin() + position<N>(in.get_padding());
    auto end = in.end() - position<N>(in.get_padding());


    do_work(in, out, op, begin, end);
//    std::cout << end << " " << in.end() << std::endl;


}

    

template<size_t N, class T, class Op>
void apply_boundary(StructuredData<N, T>& in, StructuredData<N, T>& out, [[maybe_unused]] Op op, direction<N> dir){


    fill_barrier(in, dir);

    auto begin = get_begin(in, dir);
    auto end = get_end(in, dir);

    //std::cout << dir << begin << end << in.get_dimension() << std::endl;
    /*
    std::cout << " dir " << dir << std::endl;
    std::cout << " begin " << begin << std::endl;
    std::cout << " end " << end << std::endl;
    std::cout << " dims " << in.get_dimension() << std::endl;
     */          

    do_work(in, out, op, begin, end);
    
}



template<size_t N, class T, class Op>
void apply_stencil(StructuredData<N, T>& in, StructuredData<N, T>& out, [[maybe_unused]] Op op){

    apply_interior(in, out, op);

    Neighbours<N, ConnectivityType::Star> n;    

    for (direction<N> dir : n.get()) {

        apply_boundary(in, out, op, dir);

    }
    /*
    auto begin = in.begin() + 1;
    auto end = in.end() - 1;

    for (auto pos : md_indices(begin, end)){
        out.at(pos) = op(pos, in);
    }
    */


}


/*


template<class T, class Op>
void apply_stencil(const T& in, T& out, [[maybe_unused]] Op op){


    out[0] = in[0];




}
*/
}