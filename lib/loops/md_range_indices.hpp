#pragma once

#include "loops/position.hpp"
#include "dimension.hpp"

#include <range/v3/view/indices.hpp>
#include <range/v3/view/cartesian_product.hpp>

namespace JADA{


template<size_t N>
auto md_range_indices(std::array<size_t, N> begin, std::array<size_t, N> end){

    return [&]<auto... Is>(std::index_sequence<Is...>) {
        return ranges::views::cartesian_product(
            ranges::views::indices(begin[Is], end[Is])...
        );
        //return std::array<size_t, N>{get_shift<N, Is, storage>(dimension)...};
    }(std::make_integer_sequence<size_t, N>{});


}


template<size_t N>
auto md_range_indices(position<N> begin, position<N> end){

    //using index_sequence = integer_sequence<size_t, _Idx...>;
    return [&]<idx_t... Is>(std::integer_sequence<idx_t, Is...>) {
        return ranges::views::cartesian_product(
            ranges::views::indices(begin[Is], end[Is])...
        );
        //return std::array<size_t, N>{get_shift<N, Is, storage>(dimension)...};
    }(std::make_integer_sequence<idx_t, idx_t(N)>{});


}
template<size_t N>
auto md_range_indices(dimension<N> dims){

    return md_range_indices(position<N>{}, position<N>(dims));


}





}

