#include "catch.hpp"
#include "loops/flatten_index.hpp"
#include "loops/unflatten_index.hpp"
#include "loops/md_index_loops.hpp"
#include "loops/serial_index_loops.hpp"

#include <iostream>

TEST_CASE("Test get_shift"){

    using namespace JADA;


    SECTION("1D"){
        
        size_t n0 = 10;
        dimension<1> dims{n0};

        CHECK(get_shift<1, 0, StorageOrder::RowMajor>(dims) == 1);
        CHECK(get_shift<1, 0, StorageOrder::ColMajor>(dims) == 1);

    }

    SECTION("2D"){

        size_t n0 = 10;
        size_t n1 = 11;
        dimension<2> dims{n0, n1};

        //[nj, ni]
        CHECK(get_shift<2, 0, StorageOrder::RowMajor>(dims) == n1); //ni
        CHECK(get_shift<2, 1, StorageOrder::RowMajor>(dims) == 1);


        //[ni, nj]
        CHECK(get_shift<2, 0, StorageOrder::ColMajor>(dims) == 1);
        CHECK(get_shift<2, 1, StorageOrder::ColMajor>(dims) == n0); //ni 
    }

    SECTION("3D"){

        size_t n0 = 10;
        size_t n1 = 1;
        size_t n2 = 12;
        dimension<3> dims{n0, n1, n2};


        CHECK(get_shift<3, 0, StorageOrder::RowMajor>(dims) == n1 * n2);
        CHECK(get_shift<3, 1, StorageOrder::RowMajor>(dims) == n2);
        CHECK(get_shift<3, 2, StorageOrder::RowMajor>(dims) == 1);
        
        CHECK(get_shift<3, 0, StorageOrder::ColMajor>(dims) == 1);
        CHECK(get_shift<3, 1, StorageOrder::ColMajor>(dims) == n0);
        CHECK(get_shift<3, 2, StorageOrder::ColMajor>(dims) == n0 * n1);
    }


}


TEST_CASE("Test flatten"){
    using namespace JADA;

    SECTION("1D"){

        dimension<1> dims{12};
        position<1> pos{3};
        CHECK(flatten<1, StorageOrder::RowMajor>(dims, pos) == 3);
        CHECK(flatten<1, StorageOrder::ColMajor>(dims, pos) == 3);

        CHECK(flatten<1, StorageOrder::RowMajor>(dims, {0}) == 0);
        CHECK(flatten<1, StorageOrder::ColMajor>(dims, {0}) == 0);

        CHECK(flatten<1, StorageOrder::RowMajor>(dims, {11}) == 11);
        CHECK(flatten<1, StorageOrder::ColMajor>(dims, {11}) == 11);

        REQUIRE_THROWS(
            flatten<1, StorageOrder::RowMajor>(dims, {12})
        );

        REQUIRE_THROWS(
            flatten<1, StorageOrder::ColMajor>(dims, {12})
        );
    }
    SECTION("2D"){

        dimension<2> dims {4,5};
        position<2> pos;


        pos = {0,0};
        CHECK(flatten<2, StorageOrder::RowMajor>(dims, pos) == 0);
        CHECK(flatten<2, StorageOrder::ColMajor>(dims, pos) == 0);

        pos = {1,1};
        CHECK(flatten<2, StorageOrder::RowMajor>(dims, pos) == 6);
        CHECK(flatten<2, StorageOrder::ColMajor>(dims, pos) == 5);

        pos = {5,1};
        REQUIRE_THROWS(flatten<2, StorageOrder::RowMajor>(dims, pos));
        REQUIRE_THROWS(flatten<2, StorageOrder::ColMajor>(dims, pos));
    }

    SECTION("3D"){
    
        dimension<3> dims{4,1,6};
        position<3> pos;

        pos = {0,0,0};
        CHECK(flatten<3, StorageOrder::RowMajor>(dims, pos) == 0);
        CHECK(flatten<3, StorageOrder::ColMajor>(dims, pos) == 0);

        pos = {1,0,3};
        CHECK(flatten<3, StorageOrder::RowMajor>(dims, pos) == 9);
        CHECK(flatten<3, StorageOrder::ColMajor>(dims, pos) == 13);

        pos = {1,1,3};
        REQUIRE_THROWS(flatten<3, StorageOrder::RowMajor>(dims, pos));
        REQUIRE_THROWS(flatten<3, StorageOrder::ColMajor>(dims, pos));
                

    }

    
    SECTION("4D"){
    
        dimension<4> dims{4,1,6,5};
        position<4> pos;

        pos = {0,0,0,0};
        CHECK(flatten<4, StorageOrder::RowMajor>(dims, pos) == 0);
        CHECK(flatten<4, StorageOrder::ColMajor>(dims, pos) == 0);

        pos = {1,0,3,2};
        CHECK(flatten<4, StorageOrder::RowMajor>(dims, pos) == 47);
        CHECK(flatten<4, StorageOrder::ColMajor>(dims, pos) == 61);

        pos = {1,1,3,4};
        REQUIRE_THROWS(flatten<4, StorageOrder::RowMajor>(dims, pos));
        REQUIRE_THROWS(flatten<4, StorageOrder::ColMajor>(dims, pos));
                

    }

}

TEST_CASE("Test unflatten"){

    using namespace JADA;


    SECTION("1D"){

        dimension<1> dims{7};

        CHECK(unflatten<1, StorageOrder::RowMajor>(dims, 0) == position<1>{0});
        CHECK(unflatten<1, StorageOrder::ColMajor>(dims, 0) == position<1>{0});
        
        CHECK(unflatten<1, StorageOrder::RowMajor>(dims, 3) == position<1>{3});
        CHECK(unflatten<1, StorageOrder::ColMajor>(dims, 3) == position<1>{3});

        REQUIRE_THROWS(unflatten<1, StorageOrder::RowMajor>(dims, 7));
        REQUIRE_THROWS(unflatten<1, StorageOrder::ColMajor>(dims, 7));


    }


    SECTION("2D"){
        dimension<2> dims{3, 6};

        CHECK(unflatten<2, StorageOrder::RowMajor>(dims, 0) == position<2>{0,0});
        CHECK(unflatten<2, StorageOrder::ColMajor>(dims, 0) == position<2>{0,0});
        
        CHECK(unflatten<2, StorageOrder::RowMajor>(dims, 4) == position<2>{0,4});
        CHECK(unflatten<2, StorageOrder::ColMajor>(dims, 4) == position<2>{1,1});

        CHECK(unflatten<2, StorageOrder::RowMajor>(dims, 7) == position<2>{1,1});
        CHECK(unflatten<2, StorageOrder::ColMajor>(dims, 7) == position<2>{1,2});

        CHECK(unflatten<2, StorageOrder::RowMajor>(dims, 16) == position<2>{2,4});
        CHECK(unflatten<2, StorageOrder::ColMajor>(dims, 16) == position<2>{1,5});

        REQUIRE_THROWS(unflatten<2, StorageOrder::RowMajor>(dims, 18));
        REQUIRE_THROWS(unflatten<2, StorageOrder::ColMajor>(dims, 18));


    }

    SECTION("3D"){
        dimension<3> dims{3, 1, 8};

        CHECK(unflatten<3, StorageOrder::RowMajor>(dims, 0) == position<3>{0,0,0});
        CHECK(unflatten<3, StorageOrder::ColMajor>(dims, 0) == position<3>{0,0,0});
        
        CHECK(unflatten<3, StorageOrder::RowMajor>(dims, 6) == position<3>{0,0,6});
        CHECK(unflatten<3, StorageOrder::ColMajor>(dims, 6) == position<3>{0,0,2});

        CHECK(unflatten<3, StorageOrder::RowMajor>(dims, 1) == position<3>{0,0,1});
        CHECK(unflatten<3, StorageOrder::ColMajor>(dims, 1) == position<3>{1,0,0});

        CHECK(unflatten<3, StorageOrder::RowMajor>(dims, 23) == position<3>{2,0,7});
        CHECK(unflatten<3, StorageOrder::ColMajor>(dims, 23) == position<3>{2,0,7});

        

        REQUIRE_THROWS(unflatten<3, StorageOrder::RowMajor>(dims, 24));
        REQUIRE_THROWS(unflatten<3, StorageOrder::ColMajor>(dims, 24));


    }

     SECTION("4D"){
        dimension<4> dims{3, 1, 8, 1};

        CHECK(unflatten<4, StorageOrder::RowMajor>(dims, 0) == position<4>{0,0,0,0});
        CHECK(unflatten<4, StorageOrder::ColMajor>(dims, 0) == position<4>{0,0,0,0});
        
        CHECK(unflatten<4, StorageOrder::RowMajor>(dims, 2) == position<4>{0,0,2,0});
        CHECK(unflatten<4, StorageOrder::ColMajor>(dims, 2) == position<4>{2,0,0,0});

        CHECK(unflatten<4, StorageOrder::RowMajor>(dims, 3) == position<4>{0,0,3,0});
        CHECK(unflatten<4, StorageOrder::ColMajor>(dims, 3) == position<4>{0,0,1,0});

        CHECK(unflatten<4, StorageOrder::RowMajor>(dims, 23) == position<4>{2,0,7,0});
        CHECK(unflatten<4, StorageOrder::ColMajor>(dims, 23) == position<4>{2,0,7,0});

        

        REQUIRE_THROWS(unflatten<4, StorageOrder::RowMajor>(dims, 24));
        REQUIRE_THROWS(unflatten<4, StorageOrder::ColMajor>(dims, 24));


    }
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
        for (auto [i] : serial_index(position<1>{2}, position<1>{4}, dimension<1>{10})) {
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
        for (auto [i] : serial_index(position<2>{0, 0}, position<2>{nj, ni}, dimension<2>{nj, ni})) {
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
             serial_index(position<3>{0, 0, 0}, position<3>{nk, nj, ni}, dimension<3>{nk, nj, ni})) {
            REQUIRE(i == idx);
            ++idx;
        }
        
    }
}
