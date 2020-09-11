#include "catch.hpp"

#include <array>
#include "grid/split.hpp"


TEST_CASE("Test split"){

    using namespace JADA;

    std::array<idx_t, 3> global_dims = {101, 1, 444};


    for (size_t i = 15; i < 81; ++i){
        REQUIRE_NOTHROW(split(i, global_dims));
    }




}