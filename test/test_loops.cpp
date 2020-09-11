#include "catch.hpp"

#include "loops/md_index_loops.hpp"
#include "loops/serial_index_loops.hpp"

TEST_CASE("Test loops") {

    using namespace JADA;

    SECTION("1D loops") {

        idx_t idx;

        idx = 2;
        for (auto [i] : md_indices(position<1>{2}, position<1>{4})) {
            REQUIRE(i == idx);
            ++idx;
        }

        idx = 2;
        for (auto [i] : serial_index(position<1>{2}, position<1>{4}, position<1>{10})) {
            REQUIRE(i == idx);
            ++idx;
        }
    }
    SECTION("2D loops") {

        idx_t ni = 10;
        idx_t nj = 11;

        idx_t idx;

        idx = 0;
        for (auto [j, i] : md_indices(position<2>{0, 0}, position<2>{nj, ni})) {
            REQUIRE(i + ni * j == idx);
            ++idx;
        }

        idx = 0;
        for (auto [i] : serial_index(position<2>{0, 0}, position<2>{nj, ni}, position<2>{nj, ni})) {
            REQUIRE(i == idx);
            ++idx;
        }
    }

    SECTION("3D loops") {

        idx_t ni = 10;
        idx_t nj = 11;
        idx_t nk = 12;

        idx_t idx;

        idx = 0;
        for (auto [k, j, i] : md_indices(position<3>{0, 0, 0}, position<3>{nk, nj, ni})) {
            REQUIRE(i + ni * j + ni * nj * k == idx);
            ++idx;
        }

        idx = 0;
        for (auto [i] :
             serial_index(position<3>{0, 0, 0}, position<3>{nk, nj, ni}, position<3>{nk, nj, ni})) {
            REQUIRE(i == idx);
            ++idx;
        }
    }
}
