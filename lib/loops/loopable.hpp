#pragma once

#include "loops/position.hpp"
#include "loops/md_index_loops.hpp"

namespace JADA{

template<class Derived, size_t N>
struct Loopable{

    position<N> begin() const{
        return static_cast<const Derived*>(this)->loop_begin();
    }

    position<N> end() const{
        return static_cast<const Derived*>(this)->loop_end();
    }
};


template<class Derived, size_t N>
inline static auto loop(const Loopable<Derived, N>& l){
    return md_indices(l.begin(), l.end());
}


}

