#include "catch.hpp"

#include <array>
#include <iostream>

#include "grid/decomposition.hpp"
#include "grid/local_global_mapping.hpp"
#include "grid/split.hpp"
#include "grid/uniform_grid.hpp"
#include "grid/partition.hpp"
#include "grid/create_partition.hpp"
#include "grid/domain.hpp"
#include "grid/subdomain.hpp"
#include "grid/block.hpp"
#include "grid/block_topology.hpp"
#include "operation/fdm_operations.hpp"



template <class Loop>
static std::vector<int> set_ones(const std::vector<int>& v, Loop loop) {

    std::vector<int> ret = v;
    for (auto [i] : loop) { ret[i] = 1; }
    return ret;
}


TEST_CASE("Test split") {

    using namespace JADA;

    std::array<idx_t, 3> global_dims = {101, 1, 444};

    for (size_t i = 15; i < 81; ++i) { REQUIRE_NOTHROW(split(global_dims, i)); }

    global_dims = {10, 10, 1};
    REQUIRE_THROWS(split(global_dims, 101));




    
    Block<1> parent({5},0);

    auto sub_blocks = split(parent, 5);

    for (auto s : sub_blocks){
        CHECK(s.dimensions == BlockDims<1>{1});
    }








}

TEST_CASE("Test BlockTopology"){

    using namespace JADA;
    Block<2> parent{{10, 10}, 0};

    REQUIRE_NOTHROW(BlockTopology<2>(parent, 4));

    BlockTopology<2> topo(parent, 4);

    CHECK(topo.get_children().size() == 4);
    for (auto child : topo.get_children()){
        CHECK(child.dimensions == BlockDims<2>{5,5});
    }


    std::cout << "Local: " << std::endl;
    for (auto [j,i] : topo.local_md_indices(1)){
        std::cout << j << " " << i << std::endl;
    }


    std::cout << "Global: " << std::endl;
    for (auto [j,i] : topo.global_md_indices(1)){
        std::cout << j << " " << i << std::endl;
    }


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

    std::array<size_t, 3> global_dims = {8, 8, 8};
    std::array<size_t, 3> barriers    = {0, 0, 0};
    std::array<size_t, 3> periodicity = {0, 0, 0};

    for (size_t i = 1; i < 8; ++i) {
        REQUIRE_NOTHROW(
            Decomposition<3>(i, global_dims, periodicity, barriers));
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

TEST_CASE("Test Domain"){

    using namespace JADA;

    [[maybe_unused]] std::array<std::pair<Boundary, Boundary>, 1>  all_physical_1d = {
            std::make_pair(Boundary(BoundaryType::physical, BoundaryLocation::begin),
                           Boundary(BoundaryType::physical, BoundaryLocation::end))
        };


    [[maybe_unused]] std::array<std::pair<Boundary, Boundary>, 2> all_physical_2d = {
            std::make_pair(Boundary(BoundaryType::physical, BoundaryLocation::begin),
                           Boundary(BoundaryType::physical, BoundaryLocation::end)),
            std::make_pair(Boundary(BoundaryType::physical, BoundaryLocation::begin),
                           Boundary(BoundaryType::physical, BoundaryLocation::end))
        };

    [[maybe_unused]] std::array<std::pair<Boundary, Boundary>, 3> all_physical_3d = {
            std::make_pair(Boundary(BoundaryType::physical, BoundaryLocation::begin),
                           Boundary(BoundaryType::physical, BoundaryLocation::end)),
            std::make_pair(Boundary(BoundaryType::physical, BoundaryLocation::begin),
                           Boundary(BoundaryType::physical, BoundaryLocation::end)),
            std::make_pair(Boundary(BoundaryType::physical, BoundaryLocation::begin),
                           Boundary(BoundaryType::physical, BoundaryLocation::end))

        };




    SECTION("Domain Constructors"){

        GridDims<2> dims{10,10};

        REQUIRE_NOTHROW(Domain<2>(dims, all_physical_2d));


    }

    SECTION("SubDomain<1> Constructors"){

        GridDims<1> global_dims{10};
        Domain<1> global(global_dims, all_physical_1d);        


        idx_t n_subdomains = 4;

        Decomposition<1> dec(n_subdomains, global.grid_dimensions(), global.periodic_directions(), GridDims<1>{});

        
        auto d0 = SubDomain<1>(global, n_subdomains, 0);

        CHECK(d0.grid_dimensions() == GridDims<1>{2});

        CHECK(d0.get_boundary(Direction::i, BoundaryLocation::begin).type == BoundaryType::physical);
        CHECK(d0.get_boundary(Direction::i, BoundaryLocation::end).type == BoundaryType::processor);


        auto d2 = SubDomain<1>(global, n_subdomains, 2);

        CHECK(d2.grid_dimensions() == GridDims<1>{2});

        CHECK(d2.get_boundary(Direction::i, BoundaryLocation::begin).type == BoundaryType::processor);
        CHECK(d2.get_boundary(Direction::i, BoundaryLocation::end).type == BoundaryType::processor);


        auto d3 = SubDomain<1>(global, n_subdomains, 3);

        CHECK(d3.grid_dimensions() == GridDims<1>{3});

        CHECK(d3.get_boundary(Direction::i, BoundaryLocation::begin).type == BoundaryType::processor);
        CHECK(d3.get_boundary(Direction::i, BoundaryLocation::end).type == BoundaryType::physical);


        REQUIRE_THROWS(d3.get_boundary(Direction::j, BoundaryLocation::begin));
        REQUIRE_THROWS(d3.get_boundaries(Direction::j));


        SECTION("Subdomain from subdomain"){


            auto dd3 = SubDomain<1>(d3, 2, 0 );

            CHECK(dd3.grid_dimensions() == GridDims<1>{1});


        }



    }


    SECTION("SubDomain< Constructors"){

        GridDims<2> global_dims{10,10};
        Domain<2> global(global_dims, all_physical_2d);        


        idx_t n_subdomains = 4;

        Decomposition<2> dec(n_subdomains, global.grid_dimensions(), global.periodic_directions(), GridDims<2>{});

        //"upper left"
        auto d0 = SubDomain<2>(global, n_subdomains, 0);

        CHECK(d0.grid_dimensions() == GridDims<2>{5,5});

        CHECK(d0.get_boundary(Direction::i, BoundaryLocation::begin).type == BoundaryType::physical);
        CHECK(d0.get_boundary(Direction::i, BoundaryLocation::end).type == BoundaryType::processor);

        CHECK(d0.get_boundary(Direction::j, BoundaryLocation::begin).type == BoundaryType::physical);
        CHECK(d0.get_boundary(Direction::j, BoundaryLocation::end).type == BoundaryType::processor);

        //upper right
        auto d1 = SubDomain<2>(global, n_subdomains, 1);

        CHECK(d1.grid_dimensions() == GridDims<2>{5,5});

        CHECK(d1.get_boundary(Direction::i, BoundaryLocation::end).type == BoundaryType::physical);
        CHECK(d1.get_boundary(Direction::i, BoundaryLocation::begin).type == BoundaryType::processor);

        CHECK(d1.get_boundary(Direction::j, BoundaryLocation::begin).type == BoundaryType::physical);
        CHECK(d1.get_boundary(Direction::j, BoundaryLocation::end).type == BoundaryType::processor);



    }


}