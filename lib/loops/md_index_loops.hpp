#pragma once

#include "loops/index_generator.hpp"
#include "loops/position.hpp"

namespace JADA{



[[maybe_unused]] inline static index_generator<1> 
md_indices(position<1> begin, position<1> end) noexcept {

    for (idx_t i = begin[0]; i != end[0]; ++i) {
        co_yield position<1>{i}; 
    }
}


[[maybe_unused]] inline static index_generator<2> 
md_indices(position<2> begin, position<2> end) noexcept {

    const idx_t j_begin = begin[0];
    const idx_t i_begin = begin[1];
    const idx_t j_end   = end[0];
    const idx_t i_end   = end[1];

    for (idx_t j = j_begin; j != j_end; ++j) {
    for (idx_t i = i_begin; i != i_end; ++i) { 
        
        co_yield position<2>{j, i};    
    }}
    
}



[[maybe_unused]] inline static index_generator<3> 
md_indices(position<3> begin, position<3> end) noexcept {

    const idx_t k_begin = begin[0];
    const idx_t j_begin = begin[1];
    const idx_t i_begin = begin[2];

    const idx_t k_end   = end[0];
    const idx_t j_end   = end[1];
    const idx_t i_end   = end[2];
    
    for (idx_t k = k_begin; k != k_end; ++k) {
    for (idx_t j = j_begin; j != j_end; ++j) {
    for (idx_t i = i_begin; i != i_end; ++i) { 
        
        co_yield position<3>{k, j, i}; 
    }}}
}

}