#pragma once

#include "loops/index_type.hpp"

namespace JADA{

template<class It>
struct segment{

    segment(It begin, idx_t width) : m_begin(begin), m_width(width) {}

    auto begin() {return m_begin;}
    auto end() {return m_end; }

private:
    It m_begin;
    idx_t m_width;

};




}
