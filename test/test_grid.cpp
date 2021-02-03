#include "catch.hpp"

#include <array>
#include <iostream>
#include "grid/split.hpp"
#include "grid/block_neighbours.hpp"
#include "grid/decomposition.hpp"
#include "grid/block.hpp"
#include "grid/boundary.hpp"
#include "grid/tile.hpp"
#include "grid/tile_collection.hpp"
#include "grid/tile_apply.hpp"
#include "grid/data.hpp"

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

TEST_CASE("Test Boundary"){

    using namespace JADA;

    SECTION("Constructors"){

        REQUIRE_NOTHROW(Boundary<1>());
        REQUIRE_NOTHROW(Boundary<2>({3,5}, {0,1}));
        REQUIRE_THROWS(Boundary<2>({3,5}, {0,0}));
        REQUIRE_THROWS(Boundary<2>({3,5}, {2,1}));
        REQUIRE_THROWS(Boundary<2>({3,5}, {-2,-1}));

    }

    SECTION("Loops"){

        dimension<2> dims = {2, 4};


        SECTION("Left"){

            Boundary<2> b(dims, {0,-1});
            
            std::vector<int> v1 = 
            {
                0,0,0,0,
                0,0,0,0
            };


            for (auto pos : loop(b)){
                auto idx = flatten<2, StorageOrder::RowMajor>(dims, pos);
                v1[size_t(idx)] = 1;
            }
            CHECK(v1 == 
            std::vector<int>
            {
                1,0,0,0,
                1,0,0,0
            });

        }

        SECTION("Right"){

            Boundary<2> b(dims, {0, 1});
            
            std::vector<int> v1 = 
            {
                0,0,0,0,
                0,0,0,0
            };


            for (auto pos : loop(b)){
                auto idx = flatten<2, StorageOrder::RowMajor>(dims, pos);
                v1[size_t(idx)] = 1;
            }
            CHECK(v1 == 
            std::vector<int>
            {
                0,0,0,1,
                0,0,0,1
            });

        }

        SECTION("Up"){

            Boundary<2> b(dims, {-1, 0});
            
            std::vector<int> v1 = 
            {
                0,0,0,0,
                0,0,0,0
            };


            for (auto pos : loop(b)){
                auto idx = flatten<2, StorageOrder::RowMajor>(dims, pos);
                v1[size_t(idx)] = 1;
            }
            CHECK(v1 == 
            std::vector<int>
            {
                1,1,1,1,
                0,0,0,0
            });

        }
        SECTION("Down"){

            Boundary<2> b(dims, {1, 0});
            
            std::vector<int> v1 = 
            {
                0,0,0,0,
                0,0,0,0
            };


            for (auto pos : loop(b)){
                auto idx = flatten<2, StorageOrder::RowMajor>(dims, pos);
                v1[size_t(idx)] = 1;
            }
            CHECK(v1 == 
            std::vector<int>
            {
                0,0,0,0,
                1,1,1,1
            });

        }

        SECTION("Upper left"){

            Boundary<2> b(dims, {-1, -1});
            
            std::vector<int> v1 = 
            {
                0,0,0,0,
                0,0,0,0
            };


            for (auto pos : loop(b)){
                auto idx = flatten<2, StorageOrder::RowMajor>(dims, pos);
                v1[size_t(idx)] = 1;
            }
            CHECK(v1 == 
            std::vector<int>
            {
                1,0,0,0,
                0,0,0,0
            });

        }
        SECTION("Lower right"){

            Boundary<2> b(dims, {1,1});
            
            std::vector<int> v1 = 
            {
                0,0,0,0,
                0,0,0,0
            };


            for (auto pos : loop(b)){
                auto idx = flatten<2, StorageOrder::RowMajor>(dims, pos);
                v1[size_t(idx)] = 1;
            }
            CHECK(v1 == 
            std::vector<int>
            {
                0,0,0,0,
                0,0,0,1
            });

        }
    }

}

TEST_CASE("Test Block"){

    using namespace JADA;

    SECTION("Constructors"){

        REQUIRE_NOTHROW(Block<1>());
        REQUIRE_NOTHROW(Block<2>({0,0}, {2,2}));
        REQUIRE_THROWS(Block<2>({0,0}, {5,0}));

    }

    SECTION("loop"){

        Block<1> b({0}, {3});
        std::vector<int> v(b.size(), 0);
        CHECK(v == std::vector<int>{0,0,0});

        for (auto [i] : loop(b)){
            v[size_t(i)] = 1;
        }

        CHECK(v == std::vector<int>{1,1,1});
    }

    SECTION("loop subblock"){

        Block<1> b({0}, {3});
        std::vector<int> v(b.size(), 0);
        CHECK(v == std::vector<int>{0,0,0});

        auto sb = b.get_subblock({1}, {1});

        for (auto [i] : loop(sb)){
            v[size_t(i)] = 1;
        }

        CHECK(v == std::vector<int>{0,1,0});

        REQUIRE_THROWS(b.get_subblock({0}, {4}));
        REQUIRE_THROWS(b.get_subblock({1}, {3}));


    }

    SECTION("pair loop"){

        std::vector<int> v1 = 
        {
            0,0,1,
            0,0,1
        };

        std::vector<int> v2 = 
        {
            2,0,0,
            2,0,0
        };

        Block<2> b1({0,0}, {2,3});
        Block<2> b2({0,0}, {2,3});

        for (auto [pos1, pos2] : loop(b1,b2, {0,1})){
            auto i1 = flatten<2, StorageOrder::RowMajor>(b1.dimensions(), pos1);
            auto i2 = flatten<2, StorageOrder::RowMajor>(b2.dimensions(), pos2);

            v1[size_t(i1)] = v2[size_t(i2)];

        }


        CHECK(v1 == std::vector<int>
        {
            0,0,2,
            0,0,2
        });


    }

    SECTION("get_boundary"){


        Block<2> block({0,0}, {3,4});

        std::vector<int> data = 
        {
            0,0,0,0,
            0,0,0,0,
            0,0,0,0
        };
        

        position<2>  p_begin = {1,0};
        position<2>  p_extent= {2,2};

        auto subblock = block.get_subblock(p_begin, p_extent);


        for (auto pos : loop(subblock)){

            auto idx = flatten<2, StorageOrder::RowMajor>(block.dimensions(), pos);
            data[size_t(idx)] = 1;
        }

        CHECK(data == 
        std::vector<int>
        {
            0,0,0,0,
            1,1,0,0,
            1,1,0,0
        }
        );


        for (auto pos : loop(subblock.get_boundary({-1, 0}))){

            auto idx = flatten<2, StorageOrder::RowMajor>(block.dimensions(), pos);
            data[size_t(idx)] = 2;
        }

        CHECK(data == 
        std::vector<int>
        {
            0,0,0,0,
            2,2,0,0,
            1,1,0,0
        }
        );

        for (auto pos : loop(subblock.get_boundary({0, 1}))){

            auto idx = flatten<2, StorageOrder::RowMajor>(block.dimensions(), pos);
            data[size_t(idx)] = 3;
        }

        CHECK(data == 
        std::vector<int>
        {
            0,0,0,0,
            2,3,0,0,
            1,3,0,0
        }
        );


    }

    SECTION("Block pick_data") {

        SECTION("Subblock") {

            std::vector<int> data
            {1, 1, 0, 0, 
             1, 1, 2, 0, 
             0, 0, 0, 2};

            Block<2> block({0, 0}, {3, 4});

            auto sblock1 = block.get_subblock({0, 0}, {2, 2});
            auto sblock2 = block.get_subblock({1, 2}, {2, 2});

            /*
            for (auto [j,i] : loop(sblock)){
                std::cout << j << " " <<i << std::endl;
            }
            */

            auto subset1 = pick_data(data.begin(), sblock1);
            auto subset2 = pick_data(data.begin(), sblock2);

            CHECK(subset1 == std::vector<int>{1, 1, 1, 1});
            CHECK(subset2 == std::vector<int>{2, 0, 0, 2});
        }

        SECTION("Interior Subblock"){

            SECTION("ASD"){


                Block<2> block({0, 0}, {3, 4});

                

                auto sblock1 = get_interior_subblock(block, {0,1}, 2, 1);
                auto sblock2 = get_interior_subblock(block, {0,-1}, 2, 1);
                auto sblock3 = get_interior_subblock(block, {1,0}, 1, 1);


                CHECK(sblock1.begin() == position<2>{0, 2});
                CHECK(sblock1.end() == position<2>{3, 3});

                CHECK(sblock2.begin() == position<2>{0, 2});
                CHECK(sblock2.end() == position<2>{3, 3});

                CHECK(sblock3.begin() == position<2>{1, 0});
                CHECK(sblock3.end() == position<2>{2, 4});

            }
            



            SECTION("Bug") {
                Block<2> block({0, 0}, {3, 5});

                auto sblock1 = get_interior_subblock(block, {0,1}, 2, 2);

                CHECK(sblock1.begin() == position<2>{0, 2});
                CHECK(sblock1.end() == position<2>{3, 3});

            }


        }


        SECTION("Boundary"){

            std::vector<int> data
            {1, 1, 0, 0, 
             1, 1, 2, 0, 
             0, 0, 0, 2};

            Block<2> block({0, 0}, {3, 4});
            idx_t width = 2;
            auto subset1 = pick_data(data.begin(), block, {0, 1}, width);
            auto subset2 = pick_data(data.begin(), block, {0, -1}, width);
            auto subset3 = pick_data(data.begin(), block, {1, 0}, width);
            auto subset4 = pick_data(data.begin(), block, {-1, 0}, width);
            auto subset5 = pick_data(data.begin(), block, {1, 1}, width);
            auto subset6 = pick_data(data.begin(), block, {-1, 1}, width);
            CHECK(subset1 == std::vector<int>
            {
                0,0,
                2,0,
                0,2
            });
            CHECK(subset2 == std::vector<int>
            {
                1,1,
                1,1,
                0,0
            });

            CHECK(subset3 == std::vector<int>
            {
                1,1,2,0,
                0,0,0,2
            });
            CHECK(subset4 == std::vector<int>
            {
                1,1,0,0,
                1,1,2,0
            });
            CHECK(subset5 == std::vector<int>
            {
                2,0,
                0,2
            });
            CHECK(subset6 == std::vector<int>
            {
                0,0,
                2,0
            });

        }
    }
}

TEST_CASE("Test data"){

    using namespace JADA;

    SECTION("Constructors"){



        std::vector<int> d = {1,2,3,
                              4,5,6};

        Block<2> b({0,0}, {2,3});
        Block<2> b2({0,0}, {3,3});

        REQUIRE_NOTHROW(Data(b,d));
        REQUIRE_THROWS(Data(b2,d));

    }
}

TEST_CASE("Test SplitData"){

    using namespace JADA;

    SECTION("Constructors") {

        Block<2> b1({0,0}, {2,3});
        Block<2> b2({0,0}, {3,3});

        std::vector<int> s1(b1.size());
        std::vector<int> s2(b2.size());

        REQUIRE_THROWS(SplitData(b1, b2, s1, s2, {0, 1}));
        REQUIRE_NOTHROW(SplitData(b1, b2, s1, s2, {1, 0}));


    }


}


TEST_CASE("Test orientation"){


    using namespace JADA;

    Orientation o1(0);
    Orientation o2(1);
    Orientation o3(2);
    CHECK(o1.get_direction<3>() == position<3>{1, 0, 0} );
    CHECK(o2.get_direction<3>() == position<3>{0, 1, 0} );
    CHECK(o3.get_direction<3>() == position<3>{0, 0, 1} );
    
    REQUIRE_THROWS(o3.get_direction<2>());


    Orientation o4(position<4>{0,0,1,0});

    CHECK(o4.id() == 2);



}


TEST_CASE("Test tile") {

    using namespace JADA;


    SECTION("Constructors"){

        Tile t1(-2, 2, Orientation(0));
        CHECK(t1.get_min(Orientation(0)) == -2);
        CHECK(t1.get_max(Orientation(0)) ==  2);
        CHECK(t1.get_width(Orientation(0)) == 5);


        CHECK(t1.barrier_end(Orientation(0)) == 2);
        CHECK(t1.barrier_begin(Orientation(0)) == 2);


        Tile t2(0, 3, Orientation(1));
        CHECK(t2.get_min(Orientation(1)) == 0);
        CHECK(t2.get_max(Orientation(1)) == 3);
        CHECK(t2.get_width(Orientation(1)) == 4);

        CHECK(t2.barrier_begin(Orientation(1)) == 0);
        CHECK(t2.barrier_end(Orientation(1)) == 3);


        constexpr Tile t3(-2,2,Orientation(2));

        constexpr size_t N = t3.get_width(Orientation(2));

        std::array<int, N> arr = {1,2,3,4,5};
        CHECK(arr[3] == 4);


    }


}



TEST_CASE("Test TileCollection"){

    using namespace JADA;

    SECTION("Constructors"){

        constexpr Tile t1(-2, 2, Orientation(0));
        constexpr Tile t2(-2, 2, Orientation(1));
        constexpr Tile t3(-2, 2, Orientation(2));

        constexpr TileCollection<3> s({t1,t2,t3}); 


        CHECK(s.get_tiles(Orientation(1)).size() == 1);

        constexpr Tile t4(-3, 2, Orientation(2));

        constexpr TileCollection<4> s2({t1, t2, t3, t4});

        CHECK(s2.barrier_begin(Orientation(0)) == 2);
        CHECK(s2.barrier_begin(Orientation(2)) == 3);
        CHECK(s2.barrier_end(Orientation(0)) == 2);
        CHECK(s2.barrier_end(Orientation(2)) == 2);




    }

}

TEST_CASE("Tile apply"){

    using namespace JADA;

    SECTION("1D"){

        std::vector<int> in = {1,2,3,4,5};
        std::vector<int> out = {0,0,0,0,0};

        Block<1> block({0}, {5});

        auto data_in = Data(block, in);
        auto data_out = Data(block, out);


        auto constexpr op = TEMP_OP1();

        apply_interior(data_in, data_out, op);


        CHECK(out == std::vector<int>{0, 0, (1 + 2 + 3 + 4 + 5), 0, 0});


    }

    SECTION("2D"){

        std::vector<int> in =
            {1,2,3,4,5,6, 
             1,2,3,4,5,6,
             1,2,3,4,5,6,
             1,2,3,4,5,6,
             1,2,3,4,5,6};

        std::vector<int> out(in.size());
        Block<2> block({0, 0}, {5, 6});

        auto data_in = Data(block, in);
        auto data_out = Data(block, out);


        auto constexpr op = TEMP_OP1();

        apply_interior(data_in, data_out, op);

        CHECK(out == std::vector<int>
        {
            0,0,0,0,0,0,
            0,0,0,0,0,0,
            5*1, 5*2, 5*3, 5*4, 5*5, 5*6,
            0,0,0,0,0,0,
            0,0,0,0,0,0,
        }); 

    }

}


/*
struct TEMP_OP1 {
    using Shape = JADA::Tile<-2, 2>;

    static auto apply(const auto& f){
        return f(-2) + f(-1) + f(0) + f(1) + f(2);
    } 
};

struct TEMP_OP2 {
    using Shape = JADA::Tile<1, 2>;

    static auto apply(const auto& f){
        return f(1) + f(2);
    } 
};


TEST_CASE("Test TiledData"){

    using namespace JADA;

    SECTION("Constructors"){

        std::vector<int> v = {1,2,3,4,5,6};



        REQUIRE_NOTHROW(TiledData<Tile<-2,2>, int>());


        TiledData<Tile<-2, 2>, int> d(&v[2]);

        CHECK(d(-2) == 1);
        CHECK(d(-1) == 2);
        CHECK(d(0) == 3);
        CHECK(d(1) == 4);
        CHECK(d(2) == 5);
        REQUIRE_THROWS(d(3));
        REQUIRE_THROWS(d(-3));


        TiledData<Tile<0, 3>, int> d2(&v[1]);

        CHECK(d2(0) == 2);
        CHECK(d2(1) == 3);
        CHECK(d2(2) == 4);
        CHECK(d2(3) == 5);
        REQUIRE_THROWS(d2(-1));
        REQUIRE_THROWS(d2(4));

        TiledData<Tile<-3, 0>, int> d3(&v[3]);

        CHECK(d3(-3) == 1);
        CHECK(d3(-2) == 2);
        CHECK(d3(-1) == 3);
        CHECK(d3(0) == 4);
        REQUIRE_THROWS(d3(-4));
        REQUIRE_THROWS(d3(1));


        TiledData<Tile<1, 2>, int> d4(&v[1]);
        TiledData<Tile<1, 2>, int> d5(&v[2]);
        CHECK(d4(1) == 3);
        CHECK(d4(2) == 4);
        REQUIRE_THROWS(d4(0));

        CHECK(TEMP_OP2::apply(d4) == 7);
        CHECK(TEMP_OP2::apply(d5) == 9);


    }



}
*/






