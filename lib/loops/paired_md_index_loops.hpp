#pragma once

#include "loops/index_pair_generator.hpp"

namespace JADA{

template<size_t N>
[[maybe_unused]] inline static index_pair_generator<N>
paired_md_indices(position<N> begin, position<N> end, position<N> offset) noexcept{

    for (auto pos : md_indices(begin, end)){
        co_yield {pos, pos + offset};
    }
}

}