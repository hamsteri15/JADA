#include "catch.hpp"
#include <algorithm>

#include "grid/direction.hpp"
#include "operation/fdm_operations.hpp"
#include "operation/fdm_derivative.hpp"

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



TEST_CASE("Test FdmDerivative"){

    using namespace JADA;


    SECTION("1D tests"){

        idx_t ni = 12;
        std::vector<int> in(ni);
        std::vector<int> out(ni);

        std::generate(in.begin(), in.end(), [i = 0] () mutable { i++; return i*i; });


        Grid<1> grid({ni});

        using scheme_i = FdmDerivative<1, Direction::i, DDcd2>;

        scheme_i dd_i(grid);
        
        dd_i(in, out);

        for (idx_t i = 1; i < ni - 1; ++i){
            REQUIRE(out[i] == 2);
        }

    }

    /*
    SECTION("3D tests"){

        idx_t nk = 12; idx_t nj = 11; idx_t ni = 10;
        std::vector<int> in(nk*nj*ni);
        std::vector<int> out(nk*nj*ni);

        Grid<3> grid({nk,nj,ni});

        using scheme_i = FdmDerivative<3, Direction::i, DDcd2>;
        using scheme_j = FdmDerivative<3, Direction::j, DDcd2>;
        using scheme_k = FdmDerivative<3, Direction::k, DDcd2>;


        scheme_i dd_i(grid);
        scheme_j dd_j(grid);
        scheme_k dd_k(grid);

        dd_i(in, out);
        dd_j(in, out);
        dd_k(in, out);

    }
    */


    //FdmDerivative<>



    //apply_stencil_operation<std::vector<int>, DDcd2, 3>(in, out, grid, Direction::i);

    //CHECK(out[0] != 433453);
}