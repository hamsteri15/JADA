#pragma once

#include "loops/index_type.hpp"
#include <stdexcept>


namespace JADA {

enum Direction { k = 0, j = 1, i = 2 };

template<size_t Dim>
struct DirectionMap{};

//TODO: make the function parameter constexpr when the standard allows
template<>
struct DirectionMap<1>{

    static constexpr idx_t dir_to_idx(Direction dir){
        switch (dir)
        {
        case (Direction::i):
            return 0;
        default:
            throw std::runtime_error("Invalid direction for 1D grid");
        }
        
    }

};

template<>
struct DirectionMap<2>{

    static constexpr idx_t dir_to_idx(Direction dir){
        switch (dir)
        {
        case (Direction::i):
            return 1;
        case (Direction::j):
            return 0;
        default:
            throw std::runtime_error("Invalid direction for 2D grid");
        }
        
    }

};

template<>
struct DirectionMap<3>{

    static constexpr idx_t dir_to_idx(Direction dir){
        switch (dir)
        {
        case (Direction::i):
            return 2;
        case (Direction::j):
            return 1;
        case (Direction::k):
            return 0;
        
        default:
            throw std::runtime_error("Invalid direction for 3D grid");
        }
        
    }

};








}
