#pragma once

#include "loops/dimension.hpp"
#include "loops/loopable.hpp"
#include "loops/position.hpp"

namespace JADA {

template <size_t N> struct Boundary : public Loopable<Boundary<N>, N> {

    Boundary() = default;

    Boundary(dimension<N> dims, position<N> direction) {
        // TODO: consider if this should just return begin = 0, end = 1;
        Utils::runtime_assert(direction.non_zero(),
                              "Zero direction boundary not supported");

        for (size_t i = 0; i < N; ++i) {
            Utils::runtime_assert((std::abs(direction[i]) == 1) ||
                                      (direction[i] == 0),
                                  "Elements of direction have to be +-1 or 0");

            if (direction[i] == -1) {
                m_begin[i] = 0;
                m_end[i]   = m_begin[i] + 1;
            }

            else if (direction[i] == 0) {
                m_begin[i] = 0;
                m_end[i]   = idx_t(dims[i]);
            }

            // direction[i] == 1
            else {
                m_begin[i] = idx_t(dims[i]) - 1;
                m_end[i]   = m_begin[i] + 1;
            }
        }

        m_direction = direction;

    }

    Boundary(dimension<N> dims, position<N> direction, position<N> offset) :
    Boundary(dims, direction)
    {
        m_begin += offset;
        m_end   += offset;
    }




    position<N> loop_begin() const { return m_begin; }

    position<N> loop_end() const { return m_end; }


private:
    position<N> m_begin;
    position<N> m_end;
    position<N> m_direction;
};

} // namespace JADA