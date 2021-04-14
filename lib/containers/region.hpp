#pragma once


#include "loops/dimension.hpp"
#include "loops/position.hpp"
#include "utils/runtime_assert.hpp"



namespace JADA {

template <size_t N> struct Region {

    Region() = default;

    constexpr Region(position<N> begin, position<N> end)
        : m_begin(begin)
        , m_end(end) {
            Utils::runtime_assert(begin <= end, "Begin position of region larger than end position.");
        }

    constexpr position<N>  begin() const { return m_begin; }
    constexpr position<N>  end() const { return m_end; }
    
    constexpr dimension<N> get_dimension() const {

        dimension<N> dim{};
        for (size_t i = 0; i < N; ++i){
            dim[i] = static_cast<size_t>(m_end[i] - m_begin[i]);
        }

        return dim;
    }



private:
    position<N>  m_begin;
    position<N>  m_end;
};


} // namespace JADA
