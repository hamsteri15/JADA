#include "catch.hpp"

#include <array>
#include <iostream>
#include "grid/split.hpp"
#include "grid/block_neighbours.hpp"
#include "grid/decomposition.hpp"


template <class Loop>
static std::vector<int> set_ones(const std::vector<int>& v, Loop loop) {

    std::vector<int> ret = v;
    for (auto [i] : loop) { ret[i] = 1; }
    return ret;
}




template<class T>
bool is_unique(const std::vector<T>& arr){

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

        SECTION("1D"){

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









}


TEST_CASE("Block neighbours"){

    using namespace JADA;
    using Catch::Matchers::Contains;

    SECTION("Star connectivity"){

        auto oned = block_neighbours<1, ConnectivityType::Star>();

        REQUIRE_THAT(oned,
                     Catch::Matchers::UnorderedEquals(
                         std::vector<std::array<int, 1>>{{1}, {-1}}));

        auto twod = block_neighbours<2, ConnectivityType::Star>();

        REQUIRE_THAT(
            twod,
            Catch::Matchers::UnorderedEquals(std::vector<std::array<int, 2>>{
                {1, 0}, {0, 1}, {-1, 0}, {0, -1}}));

        auto threed = block_neighbours<3, ConnectivityType::Star>();
        REQUIRE_THAT(threed,
                     Catch::Matchers::UnorderedEquals(
                         std::vector<std::array<int, 3>>{{1, 0, 0},
                                                         {0, 1, 0},
                                                         {0, 0, 1},
                                                         {-1, 0, 0},
                                                         {0, -1, 0},
                                                         {0, 0, -1}}));
    }



    SECTION("Box connecitvity"){

        auto oned = block_neighbours<1, ConnectivityType::Box>();

        CHECK_THAT(oned,
                     Catch::Matchers::UnorderedEquals(
                         std::vector<std::array<int, 1>>{{1}, {-1}}));


        auto twod = block_neighbours<2, ConnectivityType::Box>();
        CHECK(
            is_unique(twod)
        );

        CHECK_THAT(twod,
                     Catch::Matchers::UnorderedEquals(
                         std::vector<std::array<int, 2>>{
                                                        {1, 0},
                                                         {0, 1},
                                                         {1, 1},
                                                         {-1, 0},
                                                         {0, -1},
                                                         {-1, -1},
                                                         {-1, 1},
                                                         {1, -1}
                                                         }));



        CHECK(block_neighbours<3, ConnectivityType::Box>().size() == 3*3*3 - 1);
        CHECK(block_neighbours<5, ConnectivityType::Box>().size() == 3*3*3*3*3 - 1);

        CHECK(is_unique(block_neighbours<3, ConnectivityType::Box>()));
        CHECK(is_unique(block_neighbours<5, ConnectivityType::Box>()));

    }

}

/*
TEST_CASE("Test Decomposition") {

    using namespace JADA;

    std::array<size_t, 3> global_dims = {8, 8, 8};
    std::array<size_t, 3> barriers    = {0, 0, 0};
    std::array<size_t, 3> periodicity = {0, 0, 0};

    for (size_t i = 1; i < 8; ++i) {
        REQUIRE_NOTHROW(
            Decomposition<3>(i, global_dims, periodicity, barriers));
    }
}

TEST_CASE("Test Grid"){

    using namespace JADA;


    REQUIRE_NOTHROW(Grid<3>());

    Grid<3> g({1,2,3});
    //auto t = dimensions(g);

    CHECK(size(g) == 1 * 2 * 3);

    std::vector<int> v(size(g));

    for (auto [i] : indices(g)){
        v[i] = int(i);
    } 

    CHECK(v == std::vector<int>{0,1,2,3,4,5});


    for (auto [k,j,i] : md_indices(g)){
        REQUIRE(k == 0);
        REQUIRE(j <= 1);
        REQUIRE(i <= 3);
    }




}


TEST_CASE("Test UniformGrid") {

    using namespace JADA;


    SECTION("Constructors"){
        SECTION("1D"){
            Point<1> begin{};
            Point<1> end{1.0};
            GridDims<1> dims{2};

            UniformGrid<1> grid1(begin, end, dims);

            CHECK(grid1.dimensions() == dims);

            CHECK(grid1.stepsize() == std::array<double, 1>{0.5});
            CHECK(grid1.size() == 2);
            CHECK(grid1.points().size() == 2);
            CHECK(grid1.points() == std::vector<Point<1>>{
                Point<1>{0.25},
                Point<1>{0.75}
            });
        }

        SECTION("2D"){
            Point<2> begin{};
            Point<2> end{1.0, 1.0};
            GridDims<2> dims1{2, 1};

            UniformGrid<2> grid1(begin, end, dims1);

            CHECK(grid1.dimensions() == dims1);

            CHECK(grid1.stepsize() == std::array<double, 2>{0.5, 1.0});
            CHECK(grid1.size() == 2);
            CHECK(grid1.points().size() == 2);
            CHECK(grid1.points() == std::vector<Point<2>>{
                Point<2>{0.25, 0.5},
                Point<2>{0.75, 0.5}
            });



            GridDims<2> dims2{1, 1};
            UniformGrid<2> grid2(begin, end, dims2);

            CHECK(grid2.dimensions() == dims2);

            CHECK(grid2.stepsize() == std::array<double, 2>{1.0, 1.0});
            CHECK(grid2.size() == 1);
            CHECK(grid2.points().size() == 1);
            CHECK(grid2.points() == std::vector<Point<2>>{
                Point<2>{0.5, 0.5},
            });


        }

    }



    SECTION("1D loop tests"){
        std::vector<int> test = {0, 0, 0, 0};
        UniformGrid<1>          grid({test.size()});
        CHECK(grid.dimensions() == std::array<idx_t, 1>{test.size()});

        CHECK(set_ones(test, grid.get_loop()) == std::vector<int>{1,1,1,1});

    }

    SECTION("2D loop tests"){
        idx_t nj = 3; idx_t ni = 4;
        std::vector<int> test(nj*ni, 0);
        UniformGrid<2>          grid({nj, ni});
        CHECK(grid.dimensions() == std::array<idx_t, 2>{nj,ni});

        CHECK(set_ones(test, grid.get_loop()) == std::vector<int>(grid.size(), 1));

    }

    SECTION("3D loop tests"){
        idx_t nk = 2; idx_t nj = 3; idx_t ni = 4;
        std::vector<int> test(nk*nj*ni, 0);
        UniformGrid<3>          grid({nk, nj, ni});
        CHECK(grid.dimensions() == std::array<idx_t, 3>{nk,nj,ni});

        CHECK(set_ones(test, grid.get_loop()) == std::vector<int>(grid.size(), 1));

    }

}

TEST_CASE("Test Partition") {

    using namespace JADA;

    SECTION("1D tests"){

        std::array<idx_t, 1> dims{10};
        
        Partition<1> p(dims, {0}, {3});
        CHECK(p.size() == 3);
        CHECK(p.get_begin() == std::array<idx_t, 1>{0});
        CHECK(p.get_end() == std::array<idx_t, 1>{3});

        Partition<1> p2(dims, Direction::i, 5, 2);
        CHECK(p2.size() == 2);
        CHECK(p2.get_begin() == std::array<idx_t, 1>{5});
        CHECK(p2.get_end() == std::array<idx_t, 1>{7});


        REQUIRE_THROWS(Partition<1>(dims, Direction::j, 0, 2));
        REQUIRE_THROWS(Partition<1>(dims, Direction::i, 5, 7));


    }

    SECTION("2D tests"){

        idx_t ni = 12;
        idx_t nj = 10;

        std::array<idx_t, 2> dims{nj, ni};
        
        Partition<2> p(dims, {0, 0}, {0, 3});
        CHECK(p.size() == 0);
        CHECK(p.get_begin() == std::array<idx_t, 2>{0, 0});
        CHECK(p.get_end() == std::array<idx_t, 2>{0, 3});

        Partition<2> p2(dims, {0, 0}, {1, 3});
        CHECK(p2.size() == 3);
        CHECK(p2.get_begin() == std::array<idx_t, 2>{0, 0});
        CHECK(p2.get_end() == std::array<idx_t, 2>{1, 3});


        Partition<2> p3(dims, {0, 0}, dims);
        CHECK(p3.size() == 10 * 12);
        CHECK(p3.get_begin() == std::array<idx_t, 2>{0, 0});
        CHECK(p3.get_end() == dims);

        Partition<2> p4(dims, Direction::i, 5, 2);
        CHECK(p4.size() == 2 * nj);
        CHECK(p4.get_begin() == std::array<idx_t, 2>{0, 5});
        CHECK(p4.get_end() == std::array<idx_t, 2>{nj, 7});
      

        Partition<2> p5(dims, Direction::j, 5, 2);
        CHECK(p5.size() == 2 * ni);
        CHECK(p5.get_begin() == std::array<idx_t, 2>{5, 0});
        CHECK(p5.get_end() == std::array<idx_t, 2>{7, ni});


        REQUIRE_THROWS(Partition<2>(dims, Direction::k, 0, 2));
        REQUIRE_THROWS(Partition<2>(dims, Direction::i, 5, 8));



    }

    SECTION("3D tests"){

        idx_t ni = 12;
        idx_t nj = 10;
        idx_t nk = 7;
        std::array<idx_t, 3> dims{nk, nj, ni};

        Partition<3> p(dims, {0,0,0}, {1,1,1});
        CHECK(p.size()==1);
        CHECK(p.get_begin() == std::array<idx_t, 3>{0,0,0});
        CHECK(p.get_end() == std::array<idx_t, 3>{1,1,1} );


        REQUIRE_THROWS(Partition<3>(dims, Direction::k, 0, 8));

    }


    SECTION("Partition loops"){

        idx_t nj = 3; idx_t ni = 4;
        UniformGrid<2> grid({nj,ni});

        std::vector<int> test(nj*ni);
        

        SECTION("i-direction"){

            auto [begin, middle, end] = create_partitions(grid, Direction::i, DDcd2{});

            CHECK(set_ones(test, begin.get_loop()) ==
                  std::vector<int>{
                      1, 0, 0, 0, 
                      1, 0, 0, 0, 
                      1, 0, 0, 0 
                      });


            CHECK(set_ones(test, middle.get_loop()) ==
                  std::vector<int>{
                      0, 1, 1, 0, 
                      0, 1, 1, 0, 
                      0, 1, 1, 0 
                      });


            CHECK(set_ones(test, end.get_loop()) ==
                  std::vector<int>{
                      0, 0, 0, 1, 
                      0, 0, 0, 1, 
                      0, 0, 0, 1 
                      });

        }

        SECTION("j-direction"){

            auto [begin, middle, end] = create_partitions(grid, Direction::j, DDcd2{});

            CHECK(set_ones(test, begin.get_loop()) ==
                  std::vector<int>{
                      1, 1, 1, 1, 
                      0, 0, 0, 0, 
                      0, 0, 0, 0 
                      });


            CHECK(set_ones(test, middle.get_loop()) ==
                  std::vector<int>{
                      0, 0, 0, 0, 
                      1, 1, 1, 1, 
                      0, 0, 0, 0 
                      });


            CHECK(set_ones(test, end.get_loop()) ==
                  std::vector<int>{
                      0, 0, 0, 0, 
                      0, 0, 0, 0, 
                      1, 1, 1, 1 
                      });

        }


    }


}
*/

