#include "loops/md_index_loops.hpp"
#include "loops/serial_index_loops.hpp"

#pragma once

namespace JADA {

template <class GT> struct GridBase {

    // CRTP inject casts to derived types
    constexpr GT&       operator~() noexcept { 
        return static_cast<GT&>(*this); 
    }

    constexpr const GT& operator~() const noexcept {
        return static_cast<const GT&>(*this);
    }
};

// Global functions

template <typename GT> GT& crtp_cast(GridBase<GT>& grid) { 
    return ~grid; 
}

template <typename GT> const GT& crtp_cast(const GridBase<GT>& grid) {
    return ~grid;
}


/*


IMPLEMENT LOOPS HERE AS GLOBAL FUNCTIONS


*/






} // namespace JADA