#pragma once

#include <array>
#include "loops/index_type.hpp"



namespace JADA{

template<idx_t min, idx_t max>
struct Tile{

    static constexpr idx_t get_min(){
        return min;
    }

    static constexpr idx_t get_max(){
        return max;
    }

    static constexpr size_t get_width(){
        return max - min + 1;
    }

    // {0,1,2,3,4,5,6,7}
    //f[i] = f[i+1] + f[i+2] -> last index that can be computed = 5, -> barrier end = 2 + 1
    //f[i] = f[i-3] + f[i-2] + f[i-1] -> last index that can be computed = 7 -> barrier end = 0

    static constexpr idx_t barrier_end(){
        return std::min(idx_t(0), get_max());
    }

    // {0,1,2,3,4,5,6,7}
    //f[i] = f[i+1] + f[i+2] -> first index that can be computed = 0 -> barrier begin = 0
    //f[i] = f[i-3] + f[i-2] + f[i-1] -> first index that can be computed = 3 -> barrier begin = 3
    static constexpr idx_t barrier_begin(){
        return std::min(idx_t(0), -get_min());
    }






};








}