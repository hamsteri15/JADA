#include "catch.hpp"
#include <algorithm>

#include "grid/direction.hpp"
#include "operation/fdm_operations.hpp"
#include "operation/fdm_derivative.hpp"
#include "operation/fdm_boundary_operation.hpp"

namespace JADA{

struct RightBiasedOperation : public StencilOperation<RightBiasedOperation> {
        static constexpr StencilIndices<6> indices = {-2, -1, 0, 1, 2, 3};

        template <class T>
        static T
        apply(const T& v1, const T& v2, const T& v3, const T& v4, const T& v5, const T& v6) {
            return T((-v1 + 16.0 * v2 - 30.0 * v3 + 16.0 * v4 - v5 + v6) / 12.0);
        }

};

}

TEST_CASE("Test StencilOperation") {

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

TEST_CASE("Test FdmBoundaryOperation"){


    using namespace JADA;

    
    SECTION("Symmetric stencil"){
        FdmBoundaryOperation<DDcd2> b;

        CHECK(FdmBoundaryOperation<DDcd2>::scheme_indices == std::array<int, 3>{-1, 0, 1});
        CHECK(b.scheme_indices == std::array<int, 3>{-1, 0, 1});

        CHECK(b.scheme_left_width == 1);
        CHECK(b.scheme_right_width == 1);


        idx_t nk = 12; idx_t nj = 11; idx_t ni = 10;

        Grid<3> grid({nk,nj,ni});


        CHECK((nk * nj * 1) == b.get_buffer_size(grid, Direction::i, BoundaryLocation::begin));
        CHECK((nk * nj * 1) == b.get_buffer_size(grid, Direction::i, BoundaryLocation::end));

    }
    
    SECTION("Biased stencil"){

        FdmBoundaryOperation<RightBiasedOperation> b;
        CHECK(b.scheme_left_width == 2);
        CHECK(b.scheme_right_width == 3);

        idx_t nk = 12; idx_t nj = 11; idx_t ni = 10;

        Grid<3> grid({nk,nj,ni});


        CHECK((nk * nj * 2) == b.get_buffer_size(grid, Direction::i, BoundaryLocation::begin));
        CHECK((nk * nj * 3) == b.get_buffer_size(grid, Direction::i, BoundaryLocation::end));

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

    
    SECTION("2D tests"){

        idx_t nj = 11; idx_t ni = 10;
        std::vector<int> in(nj*ni);
        std::vector<int> out(nj*ni);

        Grid<2> grid({nj,ni});

        using scheme_i = FdmDerivative<2, Direction::i, DDcd2>;
        using scheme_j = FdmDerivative<2, Direction::j, DDcd2>;


        scheme_i dd_i(grid);
        scheme_j dd_j(grid);


        for (idx_t j = 0; j < nj; ++j){
        for (idx_t i = 0; i < ni; ++i){
            in[i + ni*j] = int(i*i);
        }}    

        dd_i(in, out);

        for (idx_t j = 0; j < nj; ++j){
        for (idx_t i = 1; i < ni-1; ++i){
            REQUIRE(out[i + ni*j] == 2);
        }}    


        for (idx_t j = 0; j < nj; ++j){
        for (idx_t i = 0; i < ni; ++i){
            in[i + ni*j] = int(j*j);
        }}    

        dd_j(in, out);

        for (idx_t j = 1; j < nj-1; ++j){
        for (idx_t i = 0; i < ni; ++i){
            REQUIRE(out[i + ni*j] == 2);
        }}    




    }
    

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
        
        
        //i
        for (idx_t k = 0; k < nk; ++k){
        for (idx_t j = 0; j < nj; ++j){
        for (idx_t i = 0; i < ni; ++i){
            in[i + ni*j+ni*nj*k] = int(i*i);
        }}}    

        dd_i(in, out);

        for (idx_t k = 0; k < nk; ++k){
        for (idx_t j = 0; j < nj; ++j){
        for (idx_t i = 1; i < ni-1; ++i){
            REQUIRE(out[i + ni*j+ni*nj*k] == 2);
        }}}


        //j
        for (idx_t k = 0; k < nk; ++k){
        for (idx_t j = 0; j < nj; ++j){
        for (idx_t i = 0; i < ni; ++i){
            in[i + ni*j+ni*nj*k] = int(j*j);
        }}}    

        dd_j(in, out);

        for (idx_t k = 0; k < nk; ++k){
        for (idx_t j = 1; j < nj-1; ++j){
        for (idx_t i = 0; i < ni; ++i){
            REQUIRE(out[i + ni*j+ni*nj*k] == 2);
        }}}

        //k
        for (idx_t k = 0; k < nk; ++k){
        for (idx_t j = 0; j < nj; ++j){
        for (idx_t i = 0; i < ni; ++i){
            in[i + ni*j+ni*nj*k] = int(k*k);
        }}}    

        dd_k(in, out);

        for (idx_t k = 1; k < nk-1; ++k){
        for (idx_t j = 0; j < nj; ++j){
        for (idx_t i = 0; i < ni; ++i){
            REQUIRE(out[i + ni*j+ni*nj*k] == 2);
        }}}
    }


}