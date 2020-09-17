#include "catch.hpp"

#include "operation/fdm_operations.hpp"
#include "operation/apply_stencil_operation.hpp"
#include "grid/direction.hpp"

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


TEST_CASE("apply_stencil_operation"){

    using namespace JADA;

    size_t nk = 10; size_t nj = 11; size_t ni = 12;

    std::vector<int> in(nk*nj*ni);
    std::vector<int> out(nk*nj*ni);

    Grid<3> grid({nk,nj,ni});

    auto scheme1 = DDcd2{};
    
    apply_stencil_operation(in, out, grid, Direction::i, scheme1);


}