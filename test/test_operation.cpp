#include "catch.hpp"

#include "operation/fdm_operations.hpp"

TEST_CASE("StencilOperation") {

    using namespace JADA;


    SECTION("Stencil dimensions") {

        static constexpr auto scheme1 = DDcd2{};
        static constexpr auto scheme2 = DDcd4{};

        //Note, these are compile time checks
        static_assert(scheme1.left_halfwidth() == 1);
        static_assert(scheme1.right_halfwidth() == 1);
        static_assert(scheme1.required_barriers() == 1);

        CHECK(scheme2.right_halfwidth() == 2);
        CHECK(scheme2.left_halfwidth() == 2);
        CHECK(scheme2.required_barriers() == 2);
    }
}