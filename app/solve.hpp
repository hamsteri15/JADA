#pragma once

#include "loops/position.hpp"
#include "stencil.hpp"
#include "partition.hpp"


using namespace JADA;

template<size_t N, class T>
void solve(const Partition<N, T>& in, Partition<N, T>& out, position<2> begin, position<2> end) {

    for (auto pos : md_indices(begin, end)) {

        T r(0);
        for (auto s : stencil::indices) {
            r += in[pos + s];
        }
        out[pos] = r;

    }
    
}

template<size_t N, class T>
Partition<N, T> solve(const Partition<N, T>& in, position<2> begin, position<2> end) {

    Partition<N, T> out(in);
    solve(in, out, begin, end);    
    return out;
}


