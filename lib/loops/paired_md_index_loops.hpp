#pragma once

#include "loops/index_pair_generator.hpp"

namespace JADA{



template<size_t N>
[[maybe_unused]] inline static index_pair_generator<N>
paired_md_indices(position<N> begin1, position<N> begin2, position<N> extent) noexcept{

    for (auto pos : md_indices(position<N>{}, extent)){
        co_yield {begin1 + pos, begin2 + pos};
    }
}



}