#pragma once

#include "loops/index_type.hpp"
#include "loops/position.hpp"
#include "grid/orientation.hpp"

namespace JADA{




struct Tile {

    constexpr Tile() = default;

    constexpr Tile(idx_t min, idx_t max, Orientation orr)
        : m_min(min)
        , m_max(max)
        , m_orr(orr) {}

    constexpr idx_t  get_min(Orientation o) const { 
        if (o == m_orr){
            return m_min;
        }
        return 0;
    }
    constexpr idx_t  get_max(Orientation o) const {
        if (o == m_orr){
            return m_max;
        }
        return 1;
    }
    constexpr size_t get_width(Orientation o) const {
        if (o == m_orr) {
            return size_t(m_max - m_min + 1); 
        }
        return 1; 
    }

    constexpr Orientation get_orientation() const { return m_orr; }

    
    constexpr idx_t barrier_end(Orientation o){
        return std::max(idx_t(0), get_max(o));
    }


    constexpr idx_t barrier_begin(Orientation o){
    
        return std::max(idx_t(0), -get_min(o));
    }
    


    /*
    constexpr idx_t barrier_end() const {
        return std::max(idx_t(0), get_max());
    }

    constexpr idx_t barrier_begin() const {
        return std::max(idx_t(0), -get_min());
    }
    */

private:
    idx_t  m_min, m_max;
    Orientation m_orr;
};





}