#pragma once

#include "loops/index_generator.hpp"
#include "loops/position.hpp"
#include "loops/dimension.hpp"
#include "loops/flatten_index.hpp"

namespace JADA{



[[maybe_unused]] inline static index_generator<1> 
serial_index(position<1> begin, position<1> end, [[maybe_unused]] dimension<1> dim) noexcept {

    for (auto i = begin[0]; i != end[0]; ++i) {
        co_yield position<1>{i}; 
    }
}


[[maybe_unused]] inline static index_generator<1> 
serial_index(position<2> begin, position<2> end, dimension<2> dim) noexcept {

    const auto j_begin = begin[0];
    const auto i_begin = begin[1];
    const auto j_end   = end[0];
    const auto i_end   = end[1];

    for (auto j = j_begin; j != j_end; ++j) {
    for (auto i = i_begin; i != i_end; ++i) { 

        co_yield position<1>{flatten<StorageOrder::RowMajor>(dim, j, i )}; 
        //co_yield position<1>{i+NI*j};    
    }}
    
}



[[maybe_unused]] inline static index_generator<1> 
serial_index(position<3> begin, position<3> end, dimension<3> dim) noexcept {

    const auto k_begin = begin[0];
    const auto j_begin = begin[1];
    const auto i_begin = begin[2];

    const auto k_end   = end[0];
    const auto j_end   = end[1];
    const auto i_end   = end[2];


    for (auto k = k_begin; k != k_end; ++k) {
    for (auto j = j_begin; j != j_end; ++j) {
    for (auto i = i_begin; i != i_end; ++i) { 
        
        co_yield position<1>{flatten<StorageOrder::RowMajor>(dim, k, j, i )}; 
        //co_yield position<1>{i + idx_t(NI) * j + idx_t(NINJ)*k}; 
    }}}
}


}