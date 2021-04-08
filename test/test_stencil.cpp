#include "catch.hpp"

#include "stencil/apply_stencil.hpp"
#include "containers/structured_data.hpp"
/*
#include "loops/flatten_index.hpp"
#include "stencil/stencil_indices.hpp"
#include "stencil/stencil_picker.hpp"

TEST_CASE("Test StencilIndices") {

    using namespace JADA;

    static constexpr StencilIndices<3> indices_3 = {-1, 0, 1};
    StencilIndices<5> indices_5 = {1, 2, 3, 4, 5};
    StencilIndices<6> indices_6 = {1, 2, 3, 4, 5, 6};

    SECTION("abs()") {
        CHECK(abs(indices_3) == StencilIndices<3>{1,0,1});
    }

    SECTION ("min/max()"){

        //Note this is a compile time check
        static_assert(max(indices_3) == 1);
        static_assert(min(indices_3) == -1);

        CHECK(max(indices_3) == 1);

    } 


    SECTION("reverse()") { 
        CHECK(reverse(indices_5) == StencilIndices<5>{5, 4, 3, 2, 1}); 
    }



    SECTION("mirror_front()") {

        CHECK(mirror_front(indices_5, 1) == StencilIndices<5>{3, 2, 3, 4, 5});
        CHECK(mirror_front(indices_6, 2) == StencilIndices<6>{5, 4, 3, 4, 5, 6});
        REQUIRE_THROWS(mirror_front(indices_5, 4));
    }

    SECTION("mirror_back()") {

        CHECK(mirror_back(indices_5, 1) == StencilIndices<5>{1, 2, 3, 4, 3});
        CHECK(mirror_back(indices_6, 2) == StencilIndices<6>{1, 2, 3, 4, 3, 2});
        REQUIRE_THROWS(mirror_back(indices_5, 4));
    }
}

TEST_CASE("Test StencilPicker") {

    using namespace JADA;
    SECTION("1D Pick") {

        size_t nx = 10;

        std::vector<size_t> v(nx);
        std::generate(v.begin(), v.end(), [n = 0]() mutable { return n++; });

        CHECK(v[0] == 0);
        CHECK(v[4] == 4);

        std::array<int, 3> indices     = {-1, 0, 1};
        size_t             current_idx = 2;
        size_t             shift       = 1;

        auto t = StencilPicker::pick_stencil(v, indices, current_idx, shift);

        CHECK(std::get<0>(t) == 1);
        CHECK(std::get<1>(t) == 2);
        CHECK(std::get<2>(t) == 3);
    }

    SECTION("2D Pick") {

        size_t nx = 10;
        size_t ny = 32;

        std::vector<size_t> v(nx * ny);
        std::generate(v.begin(), v.end(), [n = 0]() mutable { return n++; });

        CHECK(v[0] == 0);
        CHECK(v[4] == 4);

        std::array<int, 3> indices = {-1, 0, 1};

        size_t current_j = 5;
        size_t current_i = 5;

        size_t x_shift = 1;
        size_t y_shift = nx;

        size_t current_idx = flatten<StorageOrder::RowMajor>({ny, nx}, current_j, current_i);

        auto t_x = StencilPicker::pick_stencil(v, indices, current_idx, x_shift);
        auto t_y = StencilPicker::pick_stencil(v, indices, current_idx, y_shift);

        CHECK(std::get<0>(t_x) == current_i - 1 + nx * current_j);
        CHECK(std::get<1>(t_x) == current_i + nx * current_j);
        CHECK(std::get<2>(t_x) == current_i + 1 + nx * current_j);

        CHECK(std::get<0>(t_y) == current_i + nx * (current_j - 1));
        CHECK(std::get<1>(t_y) == current_i + nx * current_j);
        CHECK(std::get<2>(t_y) == current_i + nx * (current_j + 1));
    }

    SECTION("3D Pick") {
        using namespace JADA;


        size_t nx = 10;
        size_t ny = 32;
        size_t nz = 44;

        std::vector<size_t> v(nx * ny * nz);

        // Field<int> v(nz, 0, ny, 0, nx, 0);
        std::generate(v.begin(), v.end(), [n = 0]() mutable { return n++; });

        CHECK(v[0] == 0);
        CHECK(v[4] == 4);

        std::array<int, 3> indices = {-1, 0, 1};

        size_t current_k = 6;
        size_t current_j = 7;
        size_t current_i = 3;

        size_t x_shift = 1;
        size_t y_shift = nx;
        size_t z_shift = nx * ny;

        size_t current_idx = current_i + nx * current_j + nx * ny * current_k;

        auto t_x = StencilPicker::pick_stencil(v, indices, current_idx, x_shift);
        auto t_y = StencilPicker::pick_stencil(v, indices, current_idx, y_shift);
        auto t_z = StencilPicker::pick_stencil(v, indices, current_idx, z_shift);

        CHECK(std::get<0>(t_x) == current_i - 1 + nx * current_j + nx * ny * current_k);
        CHECK(std::get<1>(t_x) == current_i + nx * current_j + nx * ny * current_k);
        CHECK(std::get<2>(t_x) == current_i + 1 + nx * current_j + nx * ny * current_k);

        CHECK(std::get<0>(t_y) == current_i + nx * (current_j - 1) + nx * ny * current_k);
        CHECK(std::get<1>(t_y) == current_i + nx * (current_j) + nx * ny * current_k);
        CHECK(std::get<2>(t_y) == current_i + nx * (current_j + 1) + nx * ny * current_k);

        CHECK(std::get<0>(t_z) == current_i + nx * current_j + nx * ny * (current_k - 1));
        CHECK(std::get<1>(t_z) == current_i + nx * current_j + nx * ny * (current_k));
        CHECK(std::get<2>(t_z) == current_i + nx * current_j + nx * ny * (current_k + 1));
    }
}

*/

namespace JADA{

struct Op {

    template <class Some>
    auto operator()(position<2> pos, const Some& in) const {

        return in.at(pos + position<2>{0, -1}) +
               in.at(pos + position<2>{0, 1}) + 
               in.at(pos + position<2>{1, 0}) +
               in.at(pos + position<2>{-1, 0});
    }
};

}

TEST_CASE("Test apply stencil") {

    using namespace JADA;


    StructuredData<2, int> in({5,4}, {1,1});
    StructuredData<2, int> out({5,4}, {1,1});

    for (auto pos : md_indices(in.begin(), in.end())){
        in.at(pos) = 1;
    }

    fill_barriers(in);
    in.get_combined().pretty_print();

    apply_stencil(in, out, Op{});
    out.get_combined().pretty_print();


    /*

    apply_stencil(in, out, Op{});

    for (auto pos : md_indices(out.begin(), out.end())){
        CHECK(out.at(pos) == 4); 
    }
    */

    /*
    direction<2> dir;    

    dir = {1, 0};
    do_work(in, out, Op{}, get_begin(in, dir), get_end(in, dir));
    out.get_combined().pretty_print();

    dir = {0, 1};
    do_work(in, out, Op{}, get_begin(in, dir), get_end(in, dir));
    out.get_combined().pretty_print();


    dir = {0, -1};
    do_work(in, out, Op{}, get_begin(in, dir), get_end(in, dir));
    out.get_combined().pretty_print();

    dir = {1, -1};
    do_work(in, out, Op{}, get_begin(in, dir), get_end(in, dir));
    out.get_combined().pretty_print();
    */

}
