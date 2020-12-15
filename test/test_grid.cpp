#include "catch.hpp"

#include <array>
#include <iostream>
#include "grid/split.hpp"
#include "grid/block_neighbours.hpp"
#include "grid/decomposition.hpp"
#include "grid/partition.hpp"
#include "grid/boundary.hpp"
#include "grid/tiled_data.hpp"
#include "grid/tile_apply.hpp"

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

TEST_CASE("Test Partition"){

    using namespace JADA;

    SECTION("Constructors"){
        REQUIRE_NOTHROW(Partition<3>{});
        REQUIRE_NOTHROW(Partition<2>({10, 10}, {0,0}, {5,5}));
        REQUIRE_NOTHROW(Partition<2>({10, 10}, {5,5}, {5,5}));
        REQUIRE_THROWS(Partition<2>({9, 9}, {5,5}, {5,5}));

        Partition<2> parent({10, 10}, {3,3}, {5,5});

        REQUIRE_NOTHROW(Partition<2>(parent, {1,1}, {2,2}));
        REQUIRE_THROWS(Partition<2>(parent, {3,3}, {3,3}));


    }

    SECTION("loop"){

        Partition<1> p{{5}, {3}, {2}};

        std::vector<int> v = {1,2,3,4,5};

        for (auto [i] : loop(p)){
            v[size_t(i)] = -1;
        }

        CHECK(v == std::vector<int>{1,2,3,-1,-1});


    }

    SECTION("get_data"){

        std::vector<int> v = 
        {
            0,0,0,1,2,
            0,0,0,1,2,
            0,0,0,1,2
        };

        dimension<2> data_dims = {3,5};
        position<2>  p_begin = {0, 3};
        position<2>  p_extent = {3, 2};
        Partition<2> p(data_dims, p_begin, p_extent);


        CHECK(get_data(v, p) == std::vector<int>{1,2,1,2,1,2});


        std::vector<int> v2 = 
        {
            1,2,0,0,0,
            1,2,0,0,0,
            1,2,0,0,0
        };

        p_begin = {0,0};
        Partition<2> p2(data_dims, p_begin, p_extent);



        CHECK(get_data(v2, p2) == std::vector<int>{1,2,1,2,1,2});



    }

    SECTION("put_data"){

        std::vector<int> v = 
        {
            0,0,0,1,2,
            0,0,0,1,2,
            0,0,0,1,2
        };

        dimension<2> data_dims = {3,5};
        position<2>  p_begin = {0, 3};
        position<2>  p_extent = {3, 2};
        Partition<2> p(data_dims, p_begin, p_extent);

        std::vector<int> p_data = {3,3,3,3,3,3};

        put_data(v, p_data, p);

        CHECK(v == 
            std::vector<int>
            {
                0,0,0,3,3,
                0,0,0,3,3,
                0,0,0,3,3
            });
            



        p_begin = {0,0};
        Partition<2> p2(data_dims, p_begin, p_extent);


        put_data(v, p_data, p2);

        CHECK(v == 
            std::vector<int>
            {
                3,3,0,3,3,
                3,3,0,3,3,
                3,3,0,3,3
            });



    }




    SECTION("test swap"){

        //k,j,i
        dimension<2> dim1 = {3,3};
        position<2>  p1_begin = {0, 2};
        position<2>  p1_extent = {3, 1};

        Partition<2> p1(dim1, p1_begin, p1_extent);

        std::vector<int> v1 = 
        {
            1,2,3,
            1,2,3,
            1,2,3
        };

        auto p1_data = get_data(v1, p1);

        /////

        dimension<2> dim2 = {3, 4};
        position<2> p2_begin = {0,0};
        position<2> p2_extent = {3, 1};

        Partition<2> p2(dim2, p2_begin, p2_extent);        

        std::vector<int> v2 = 
        {
            2,3,4,5,
            2,3,4,5,
            2,3,4,5
        };

        put_data(v2, p1_data, p2);

        CHECK( v2 ==
            std::vector<int>
            {
                3,3,4,5,
                3,3,4,5,
                3,3,4,5
            }
        );

        p2_begin = {0, 3};
        Partition<2> p3(dim2, p2_begin, p2_extent);

        put_data(v2, p1_data, p3);

        CHECK( v2 ==
            std::vector<int>
            {
                3,3,4,3,
                3,3,4,3,
                3,3,4,3
            }
        );


    }



    SECTION("get_boundary"){

        dimension<2> global_d = {3, 4};
        
        std::vector<int> data = 
        {
            0,0,0,0,
            0,0,0,0,
            0,0,0,0
        };
        

        position<2>  p_begin = {1,0};
        position<2>  p_extent= {2,2};

        Partition<2> p(global_d, p_begin, p_extent);


        for (auto pos : loop(p)){

            auto idx = flatten<2, StorageOrder::RowMajor>(global_d, pos);
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



        for (auto pos : loop(p.get_boundary({-1, 0}))){

            auto idx = flatten<2, StorageOrder::RowMajor>(global_d, pos);
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


        for (auto pos : loop(p.get_boundary({0, 1}))){

            auto idx = flatten<2, StorageOrder::RowMajor>(global_d, pos);
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


    SECTION("Paired loop"){

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

        dimension<2> d1 = {2,3};
        dimension<2> d2 = {2,3};
        Partition<2> p1(d1, {0,0}, {2,3});
        Partition<2> p2(d2, {0,0}, {2,3});

        for (auto [pos1, pos2] : loop(p1,p2, {0,1})){
            auto i1 = flatten<2, StorageOrder::RowMajor>(d1, pos1);
            auto i2 = flatten<2, StorageOrder::RowMajor>(d2, pos2);

            v1[size_t(i1)] = v2[size_t(i2)];

        }


        CHECK(v1 == std::vector<int>
        {
            0,0,2,
            0,0,2
        });


    }


}

TEST_CASE("Test tile") {

    using namespace JADA;




}

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





    }



}

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


TEST_CASE("Tile apply"){

    using namespace JADA;


    /*
    SECTION("1D"){


        std::vector<int> in = {1,2,3,4,5};
        std::vector<int> out = {0,0,0,0,0};

        Partition<1> p({5}, {2}, {1});

        apply(in, out, p, CD4());

        CHECK(out[2] == 1 + 2 + 3 + 4 + 5);

        for (auto o : out){
            std::cout << o << std::endl;
        }


    }
    */

   
    SECTION("Boundary apply centered stencil"){

        std::vector<int> in1 = {1,2,3,4,5};
        std::vector<int> in2 = {6,7};
        std::vector<int> out = {0,0,0,0,0};

        Partition<1> p1({5}, {0}, {5});
        Partition<1> p2({2}, {0}, {2});


       
        apply(in1, in2, out, p1, p2, {1}, TEMP_OP1{});

        CHECK(out ==
        std::vector<int>
        {
            0,
            0,
            0,
            in1[1] + in1[2] + in1[3] + in1[4] + in2[0],
            in1[2] + in1[3] + in1[4] + in2[0] + in2[1]
        });

        //CHECK(out == std::vector<int>{0,0,0, 4,5});


    }


    /*

    SECTION("Boundary apply right biased stencil"){

        //f[i] = f[i+1] + f[i+2]

        std::vector<int> in1 = {1,2,3,4,5};
        std::vector<int> in2 = {6,7,8,9};
        std::vector<int> out = {0,0,0,0,0};

        Partition<1> p1({5}, {0}, {5});
        Partition<1> p2({4}, {0}, {4});


        //
       
        apply(in1, in2, out, p1, p2, {1}, TEMP_OP2{});

        CHECK(out ==
        std::vector<int>
        {
            0,
            0,
            0,
            in1[4] + in2[0],
            in2[1] + in2[2]
        });

        //CHECK(out == std::vector<int>{0,0,0, 4,5});

    }
    */



}



