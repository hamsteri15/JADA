#pragma once

#include <type_traits>
#include <array>

namespace JADA::Utils{

template<class new_t, class old_t, std::size_t N>
static constexpr std::array<new_t, N> array_cast(std::array<old_t, N> old){

    static_assert(std::is_convertible<old_t, new_t>::value, "Old type cannot be casted to new type");

    std::array<new_t, N> new_arr;
    /*
    for (std::size_t i = 0; i < N; ++i){
        new_arr[i] = static_cast<new_t>(old[i]);
    }
    */
   std::copy(old.begin(), old.end(), new_arr.begin());
    

    return new_arr;
}



}