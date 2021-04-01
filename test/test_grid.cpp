#include "catch.hpp"

#include <array>
#include <iostream>
#include "grid/split.hpp"
#include "grid/neighbours.hpp"
#include "grid/decomposition.hpp"

template <class Loop>
static std::vector<int> set_ones(const std::vector<int>& v, Loop loop) {

    std::vector<int> ret = v;
    for (auto [i] : loop) { ret[i] = 1; }
    return ret;
}




template<class Container>
bool is_unique(const Container& arr){

    auto copy = arr;
    std::sort(std::begin(copy), std::end(copy));
    auto it = std::unique( copy.begin(), copy.end() );
    bool isUnique = (it == copy.end() );

    return isUnique;
}

TEST_CASE("Test split") {

    using namespace JADA;

    dimension<3> global_dims = {101, 1, 444};

    for (size_t i = 15; i < 81; ++i) { REQUIRE_NOTHROW(split(global_dims, i)); }

    global_dims = {10, 10, 1};
    REQUIRE_THROWS(split(global_dims, 101));
}

TEST_CASE("Test Decomposition"){

    using namespace JADA;

    SECTION("Constructors"){
        REQUIRE_NOTHROW(Decomposition<2>());
        REQUIRE_NOTHROW(Decomposition<2>({100, 100}, 4, {true, true}));
        REQUIRE_THROWS(Decomposition<2>({1,1}, 4, {true, true}));

        REQUIRE_NOTHROW(Decomposition<2>({100, 100}, {10, 10}, {true, true}));
        REQUIRE_THROWS(Decomposition<2>({100, 100}, {111, 1}, {true, true}));

    }

    SECTION("local_extent()") {


        SECTION("Even splitting"){
            dimension<3> n_nodes  = {4,4,4};
            Decomposition<3> dec(n_nodes, 8, {true, true});

            CHECK(dec.dimensions() == dimension<3>{2,2,2});


            for (size_t i = 0; i < 8; ++i){
                REQUIRE(dec.local_grid_dimensions(idx_t(i)) == dimension<3>{2,2,2});
            }

        }

        SECTION("Uneven splitting"){

            SECTION("1d"){
                dimension<1> n_nodes  = {4};
                Decomposition<1> dec(n_nodes, 3, {true});
                
                CHECK(dec.dimensions() == dimension<1>{3});

                CHECK(dec.local_grid_dimensions(0) == dimension<1>{1});
                CHECK(dec.local_grid_dimensions(1) == dimension<1>{1});
                CHECK(dec.local_grid_dimensions(2) == dimension<1>{2});

                REQUIRE_THROWS(dec.local_grid_dimensions(3));
                REQUIRE_THROWS(dec.local_grid_dimensions(-1));

            }
            SECTION("3d"){
                dimension<3> n_nodes  = {5,4,4};
                Decomposition<3> dec(n_nodes, 8, {true, true});
                
                CHECK(dec.dimensions() == dimension<3>{2,2,2});

                CHECK(dec.local_grid_dimensions(0) == dimension<3>{2,2,2});
                CHECK(dec.local_grid_dimensions(1) == dimension<3>{2,2,2});
                CHECK(dec.local_grid_dimensions(2) == dimension<3>{2,2,2});
                CHECK(dec.local_grid_dimensions(3) == dimension<3>{2,2,2});
                CHECK(dec.local_grid_dimensions(4) == dimension<3>{3,2,2});
                CHECK(dec.local_grid_dimensions(5) == dimension<3>{3,2,2});
                CHECK(dec.local_grid_dimensions(6) == dimension<3>{3,2,2});
                CHECK(dec.local_grid_dimensions(7) == dimension<3>{3,2,2});

                REQUIRE_THROWS(dec.local_grid_dimensions(8));
                REQUIRE_THROWS(dec.local_grid_dimensions(-1));

            }

        }


    }

    SECTION("get_offset()"){

        SECTION("Even splitting"){
            SECTION("1d"){
                dimension<1> n_nodes  = {4};
                Decomposition<1> dec(n_nodes, 4, {true});
                CHECK(dec.dimensions() == dimension<1>{4});

                CHECK(dec.get_offset(0) == dimension<1>{0});
                CHECK(dec.get_offset(1) == dimension<1>{1});
                CHECK(dec.get_offset(2) == dimension<1>{2});
                CHECK(dec.get_offset(3) == dimension<1>{3});

            }
        }
        SECTION("Uneven splitting"){
            SECTION("1d"){
                dimension<1> n_nodes  = {5};
                Decomposition<1> dec(n_nodes, 4, {true});
                CHECK(dec.dimensions() == dimension<1>{4});

                CHECK(dec.get_offset(0) == dimension<1>{0});
                CHECK(dec.get_offset(1) == dimension<1>{1});
                CHECK(dec.get_offset(2) == dimension<1>{2});
                CHECK(dec.get_offset(3) == dimension<1>{3});

            }
        }

    }

    SECTION("get_neighbour()"){

        SECTION("Default constructed") {

            Decomposition<2> dec;

            REQUIRE_THROWS(dec.get_neighbour(0, {0,1}));


        }


        SECTION("1D"){

            SECTION("Self-send periodicity") {

                Decomposition<1> dec({5}, {1}, {true});
                CHECK(dec.get_neighbour(0, {1}) == 0);
                CHECK(dec.get_neighbour(0, {-1}) == 0);

            }


            SECTION("Periodic"){
                dimension<1> n_nodes  = {5};
                Decomposition<1> dec(n_nodes, 4, {true});
                CHECK(dec.get_neighbour(0, {1}) == 1);
                CHECK(dec.get_neighbour(0, {-1}) == 3);
                CHECK(dec.get_neighbour(1, {1}) == 2);

                CHECK(dec.get_neighbour(3, {1}) == 0);
                CHECK(dec.get_neighbour(3, {4}) == 3);

                REQUIRE_THROWS(dec.get_neighbour(3, {5}));
                REQUIRE_THROWS(dec.get_neighbour(3, {-5}));



                dimension<1> n_nodes2 = {5};
                Decomposition<1> dec2(n_nodes2, 1, {true});
                CHECK(dec2.get_neighbour(0, {1}) == 0); 
                CHECK(dec2.get_neighbour(0, {-1}) == 0); 
                REQUIRE_THROWS(dec2.get_neighbour(0, {2}) == 0); 
                REQUIRE_THROWS(dec2.get_neighbour(0, {-2}) == 0); 


            }
            SECTION("Non-periodic"){
                dimension<1> n_nodes  = {5};
                Decomposition<1> dec(n_nodes, 4, {false});
                CHECK(dec.get_neighbour(0, {1}) == 1);
                CHECK(dec.get_neighbour(0, {-1}) == NEIGHBOUR_ID_NULL);
                CHECK(dec.get_neighbour(1, {1}) == 2);

                CHECK(dec.get_neighbour(3, {1}) == NEIGHBOUR_ID_NULL);

            }

        }


    }

    SECTION("2D") {
        SECTION("Periodic self-send") {

            Decomposition<2> dec({10,10}, {1,1}, {true, true});

            CHECK(dec.get_neighbour(0, {0,1}) == 0);
            CHECK(dec.get_neighbour(0, {0,-1}) == 0);
            CHECK(dec.get_neighbour(0, {1,0})== 0);
            CHECK(dec.get_neighbour(0, {-1,0}) == 0);


        }
    }



}


TEST_CASE("Block neighbours"){

    

    using namespace JADA;
    using Catch::Matchers::Contains;
    
    SECTION("Neighbours"){

        REQUIRE_NOTHROW(Neighbours<3, ConnectivityType::Star>());
        REQUIRE_NOTHROW(Neighbours<3, ConnectivityType::Box>());
        
        CHECK(is_unique(Neighbours<3, ConnectivityType::Box>().get()));
        CHECK(is_unique(Neighbours<2, ConnectivityType::Star>().get()));
        CHECK(is_unique(Neighbours<4, ConnectivityType::Box>().get()));



        Neighbours<2, ConnectivityType::Star> ns1;
        Neighbours<2, ConnectivityType::Box> ns2;

//        static constexpr auto ns_const = Neighbours<2, ConnectivityType::Star>();

        constexpr Neighbours<2, ConnectivityType::Star> ns_const;


        static_assert(Neighbours<2, ConnectivityType::Star>::idx({0,1}) == 0, "Neighbours not constexpr");

        CHECK(ns1.idx({0,1}) == 0);
        CHECK(ns2.idx({0,1}) == 0);
        static_assert(ns_const.idx({0,1}) == 0, "Neighbours not constexpr");



    }




    SECTION("Star connectivity"){

        SECTION("1D") {
            auto test = Neighbours<1, ConnectivityType::Star>().get();

            std::array<std::array<idx_t, 1>, 2> correct
            {
                std::array<idx_t,1>{1}, std::array<idx_t,1>{-1}
            };

            std::sort(test.begin(), test.end());
            std::sort(correct.begin(), correct.end());

            CHECK(test == correct);

        }


        SECTION("2D") {
            auto test = Neighbours<2, ConnectivityType::Star>().get();

            std::array<std::array<idx_t, 2>, 4> correct
            {
                std::array<idx_t, 2>{1, 0}, 
                std::array<idx_t, 2>{0, 1},
                std::array<idx_t, 2>{-1, 0}, 
                std::array<idx_t, 2>{0, -1}
            };

            std::sort(test.begin(), test.end());
            std::sort(correct.begin(), correct.end());

            CHECK(test == correct);

            test[2][0] = 3;
            CHECK(test != correct);


        }

        SECTION("3D") {
            auto test = Neighbours<3, ConnectivityType::Star>().get();

            std::array<std::array<idx_t, 3>, 6> correct
            {
                std::array<idx_t, 3>{1, 0, 0},
                std::array<idx_t, 3>{0, 1, 0},
                std::array<idx_t, 3>{0, 0, 1},
                std::array<idx_t, 3>{-1, 0, 0},
                std::array<idx_t, 3>{0, -1, 0},
                std::array<idx_t, 3>{0, 0, -1}
                
            };

            std::sort(test.begin(), test.end());
            std::sort(correct.begin(), correct.end());

            CHECK(test == correct);

            test[2][0] = 3;
            CHECK(test != correct);


        }


    }

    SECTION("Box connecitvity"){

        SECTION("1D") {
            auto test = Neighbours<1, ConnectivityType::Box>().get();
            
            std::array<std::array<idx_t, 1>, 2> correct
            {
                std::array<idx_t,1>{1}, std::array<idx_t,1>{-1}
            };

            std::sort(test.begin(), test.end());
            std::sort(correct.begin(), correct.end());

            CHECK(test == correct);


        }


        SECTION("2D") {
            auto test = Neighbours<2, ConnectivityType::Box>().get();
            
            std::array<std::array<idx_t, 2>, 8> correct
            {
                std::array<idx_t, 2>{1, 0},
                std::array<idx_t, 2>{0, 1},
                std::array<idx_t, 2>{1, 1},
                std::array<idx_t, 2>{-1, 0},
                std::array<idx_t, 2>{0, -1},
                std::array<idx_t, 2>{-1, -1},
                std::array<idx_t, 2>{-1, 1},
                std::array<idx_t, 2>{1, -1}
                
            };

            std::sort(test.begin(), test.end());
            std::sort(correct.begin(), correct.end());

            CHECK(test == correct);



        }


    }

    SECTION("Neighbours idx()"){

        Neighbours<2, ConnectivityType::Star> n;

        //The ordere shouldnt matter here so this is bit of a bad test...
        CHECK(n.idx({0,1}) == 0);
        CHECK(n.idx({1,0}) == 1);
        CHECK(n.idx({-1,0}) == 2);
        CHECK(n.idx({0,-1}) == 3);



    }



}


