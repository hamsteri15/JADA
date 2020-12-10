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

};


}