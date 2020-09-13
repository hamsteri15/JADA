#include "catch.hpp"

#include <array>

#include "grid/split.hpp"
#include "grid/decomposition.hpp"
#include "grid/local_global_mapping.hpp"
#include "grid/uniform_grid.hpp"

TEST_CASE("Test split") {

    using namespace JADA;

    std::array<idx_t, 3> global_dims = {101, 1, 444};

    for (size_t i = 15; i < 81; ++i) { REQUIRE_NOTHROW(split(i, global_dims)); }

    global_dims = {10, 10, 1};
    REQUIRE_THROWS(split(101, global_dims));
}

TEST_CASE("Test LocalGlobalMapping") {

    using namespace JADA;

    SECTION("local_extent()") {

        LocalGlobalMapping<3> map;

        std::array<size_t, 3> n_nodes  = {10, 10, 10};
        std::array<size_t, 3> n_splits = {3, 3, 3};

        std::array<size_t, 3> coords = {0, 0, 0};
        CHECK(map.local_extent(n_nodes, n_splits, coords) ==
              std::array<size_t, 3>{3, 3, 3});

        coords = {1, 1, 1};
        CHECK(map.local_extent(n_nodes, n_splits, coords) ==
              std::array<size_t, 3>{3, 3, 3});

        coords = {2, 2, 2};
        CHECK(map.local_extent(n_nodes, n_splits, coords) ==
              std::array<size_t, 3>{4, 4, 4});

        n_nodes  = {10, 10, 10};
        n_splits = {2, 2, 2};

        coords = {0, 0, 0};
        CHECK(map.local_extent(n_nodes, n_splits, coords) ==
              std::array<size_t, 3>{5, 5, 5});

        coords = {1, 0, 1};
        CHECK(map.local_extent(n_nodes, n_splits, coords) ==
              std::array<size_t, 3>{5, 5, 5});

        n_nodes  = {7, 7, 7};
        n_splits = {3, 3, 3};

        coords = {0, 0, 0};
        CHECK(map.local_extent(n_nodes, n_splits, coords) ==
              std::array<size_t, 3>{2, 2, 2});

        coords = {1, 1, 1};
        CHECK(map.local_extent(n_nodes, n_splits, coords) ==
              std::array<size_t, 3>{2, 2, 2});

        coords = {2, 2, 2};
        CHECK(map.local_extent(n_nodes, n_splits, coords) ==
              std::array<size_t, 3>{3, 3, 3});

        n_nodes  = {8, 8, 8};
        n_splits = {2, 2, 2};
        coords   = {0, 0, 0};
        CHECK(map.local_extent(n_nodes, n_splits, coords) ==
              std::array<size_t, 3>{4, 4, 4});
    }

    SECTION("start()") {

        LocalGlobalMapping<3> map;

        std::array<size_t, 3> n_nodes  = {10, 10, 10};
        std::array<size_t, 3> n_splits = {2, 2, 2};
        std::array<size_t, 3> coords;

        coords = {0, 0, 0};
        CHECK(map.start(n_nodes, n_splits, coords) ==
              std::array<size_t, 3>{0, 0, 0});

        coords = {1, 0, 0};
        CHECK(map.start(n_nodes, n_splits, coords) ==
              std::array<size_t, 3>{5, 0, 0});

        n_nodes  = {10, 10, 10};
        n_splits = {3, 2, 2};

        coords = {1, 0, 0};
        CHECK(map.start(n_nodes, n_splits, coords) ==
              std::array<size_t, 3>{3, 0, 0});

        coords = {2, 0, 0};
        CHECK(map.start(n_nodes, n_splits, coords) ==
              std::array<size_t, 3>{6, 0, 0});
    }

    SECTION("global_to_local()") {

        LocalGlobalMapping<3> map;

        std::array<size_t, 3> n_nodes  = {10, 10, 10};
        std::array<size_t, 3> n_splits = {3, 3, 3};
        std::array<size_t, 3> global_idx;

        global_idx = {0, 0, 0};
        auto p1    = map.global_to_local(n_nodes, n_splits, global_idx);
        CHECK(p1.first == std::array<size_t, 3>{0, 0, 0});  // coords
        CHECK(p1.second == std::array<size_t, 3>{0, 0, 0}); // indices

        global_idx = {2, 5, 6};
        auto p2    = map.global_to_local(n_nodes, n_splits, global_idx);
        CHECK(p2.first == std::array<size_t, 3>{0, 1, 2});  // coords
        CHECK(p2.second == std::array<size_t, 3>{2, 2, 0}); // indices
    }
}

TEST_CASE("Test Decomposition") {

    using namespace JADA;


        std::array<size_t, 3> global_dims = {8,8,8};
    std::array<size_t, 3> barriers = {0,0,0};
    std::array<size_t, 3> periodicity = {0,0,0};

    for (size_t i = 1; i < 8; ++i){
        REQUIRE_NOTHROW(Decomposition<3>(i, global_dims, periodicity, barriers));
    }

}


TEST_CASE("Test UniformGrid") {

    using namespace JADA;


    std::array<double, 2> L{1.0, 1.0};
    std::array<size_t, 2> dims{10, 10};

    UniformGrid<2> grid(dims, L);

    CHECK(grid.stepsize() == std::array<double, 2>{1.0/10, 1.0/10});

}