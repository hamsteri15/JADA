#include "catch.hpp"
#include "loops/flatten_index.hpp"
#include "loops/md_index_loops.hpp"
#include "loops/serial_index_loops.hpp"

TEST_CASE("Test flatten"){

    using namespace JADA;


    SECTION("1D"){
        
        idx_t n0 = 10;
        std::array<idx_t, 1> dims{n0};

        CHECK(get_shift<1, 0, StorageOrder::RowMajor>(dims) == 1);
        CHECK(get_shift<1, 0, StorageOrder::ColMajor>(dims) == 1);

    }

    SECTION("2D"){

        idx_t n0 = 10;
        idx_t n1 = 11;
        std::array<idx_t, 2> dims{n0, n1};

        //[nj, ni]
        CHECK(get_shift<2, 0, StorageOrder::RowMajor>(dims) == n1); //ni
        CHECK(get_shift<2, 1, StorageOrder::RowMajor>(dims) == 1);


        //[ni, nj]
        CHECK(get_shift<2, 0, StorageOrder::ColMajor>(dims) == 1);
        CHECK(get_shift<2, 1, StorageOrder::ColMajor>(dims) == n0); //ni 
    }

    SECTION("3D"){

        idx_t n0 = 10;
        idx_t n1 = 1;
        idx_t n2 = 12;
        std::array<idx_t, 3> dims{n0, n1, n2};


        CHECK(get_shift<3, 0, StorageOrder::RowMajor>(dims) == n1 * n2);
        CHECK(get_shift<3, 1, StorageOrder::RowMajor>(dims) == n2);
        CHECK(get_shift<3, 2, StorageOrder::RowMajor>(dims) == 1);
        
        CHECK(get_shift<3, 0, StorageOrder::ColMajor>(dims) == 1);
        CHECK(get_shift<3, 1, StorageOrder::ColMajor>(dims) == n0);
        CHECK(get_shift<3, 2, StorageOrder::ColMajor>(dims) == n0 * n1);
    }


    idx_t nk = 1;
    idx_t nj = 5;
    idx_t ni = 13;
    std::array<idx_t, 3> dims{nk, nj, ni};

    idx_t i = 3;
    idx_t j = 2;
    idx_t k = 0;

    CHECK(flatten<StorageOrder::RowMajor>(dims, k,j,i) == k*nj*ni + ni * j + i);


    int ii = 3;
    int jj = 2;
    int kk = 0;

    CHECK(flatten<StorageOrder::RowMajor>(dims, kk, jj, ii) == idx_t(kk)*nj*ni + ni * idx_t(jj) + idx_t(ii));




}


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
