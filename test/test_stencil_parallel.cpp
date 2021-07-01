#include "catch.hpp"

#include <vector>
#include <array>

#include <hpx/algorithm.hpp>
#include <hpx/chrono.hpp>
#include <hpx/execution.hpp>
#include <hpx/future.hpp>
#include <hpx/include/components.hpp>
#include <hpx/include/compute.hpp>
#include <hpx/include/util.hpp>
#include <hpx/iostream.hpp>

#include "communication/md_communicator.hpp"
#include "communication/hpx_md_communicator.hpp"
#include "grid/neighbours.hpp"
#include "stencil/apply_stencil.hpp"
#include "stencil/apply_stencil_pvec.hpp"
#include "stencil/op_to_iterators.hpp"

#include "containers/partitioned_vector.hpp"

#include "loops/positions_to_iterators.hpp"

#include "ops.hpp"




/*
TEST_CASE("Test apply_stencil_parallel"){

    using namespace JADA;


    SECTION("1D linear"){

        using comm_data_t = std::vector<int>;
        using comm_t = HpxMdCommunicator<comm_data_t, 1, ConnectivityType::Box>;
        std::string comm_name = "oned_stencil_tile";

        size_t num_local_partitions = 8;
        size_t num_localities = hpx::get_num_localities(hpx::launch::sync);
        size_t n_partitions = num_local_partitions * num_localities;
        size_t rank = hpx::get_locality_id();

        dimension<1> global_dims = {80};
        std::array<bool, 1> periodicity  = {true};

        Decomposition<1> dec(
            global_dims,
            n_partitions,
            periodicity
        );

        CHECK(dec.global_grid_dimensions().elementwise_product() != size_t(0));

        std::vector<comm_t> comms;
        std::vector<comm_data_t> i_data;
        std::vector<comm_data_t> o_data;

        for (size_t i = 0; i < num_local_partitions; ++i){

            idx_t id = idx_t(rank * num_local_partitions + i);
            comms.push_back(
                comm_t(comm_name, id, dec)
            );

            auto local_dims = dec.local_grid_dimensions(id);
            auto local_begin  = dec.get_offset(id);
            

            comm_data_t in(local_dims.elementwise_product());
            comm_data_t out(local_dims.elementwise_product(), 0);
            for (size_t ii = 0; ii < in.size(); ++ii){
                idx_t idx = int(ii) + int(local_begin[0]);
                in[ii] = int(idx * idx); //derivative should be two
                
            }

            i_data.push_back(in);
            o_data.push_back(out);

            call_sets(i_data[i], local_dims, OneDLinear{}, comms[i], size_t(0));

        }


        size_t n_steps = 1;

        for (size_t n = 0; n < n_steps; ++n){
            
            size_t i = 0;
            for (auto comm : comms) {

                auto dims = comm.get_decomposition().local_grid_dimensions(comm.id());
                apply_stencil(i_data[i], o_data[i], dims, OneDLinear{}, comm, n );
                ++i;

            }


            //skip the first and last because of unhandled boundaries
            for (auto it = o_data.begin() + 1; it != o_data.end() - 1; ++it){
                for (auto e : *it){
                    REQUIRE(e == 2);
                }
            }
        

        }




    }


    SECTION("Box operation"){

        
        using comm_data_t = std::vector<int>;
        using comm_t = HpxMdCommunicator<comm_data_t, 2, ConnectivityType::Box>;
        std::string comm_name = "2by2_stencil_box";

        size_t num_local_partitions = 5;
        size_t num_localities = hpx::get_num_localities(hpx::launch::sync);
        size_t n_partitions = num_local_partitions * num_localities;
        size_t rank = hpx::get_locality_id();

        dimension<2> global_dims = {10, 10};
        std::array<bool, 2> periodicity  = {true, true};

        
        Decomposition<2> dec(
            global_dims,
            n_partitions,
            periodicity
        );

        CHECK(dec.global_grid_dimensions().elementwise_product() != size_t(0));

        std::vector<comm_t> comms;
        std::vector<comm_data_t> i_data;
        std::vector<comm_data_t> o_data;


        for (size_t i = 0; i < num_local_partitions; ++i){

            idx_t id = idx_t(rank * num_local_partitions + i);
            comms.push_back(
                comm_t(comm_name, id, dec)
            );

            auto local_dims = dec.local_grid_dimensions(id);
            i_data.push_back(comm_data_t(local_dims.elementwise_product(), 1));
            o_data.push_back(comm_data_t(local_dims.elementwise_product(), 0));

            call_sets(i_data[i], local_dims, OpBox{}, comms[i], size_t(0));
            

        }

        



        size_t n_steps = 10;

        for (size_t n = 0; n < n_steps; ++n){
            
            size_t i = 0;
            for (auto comm : comms) {

                auto dims = comm.get_decomposition().local_grid_dimensions(comm.id());
                apply_stencil(i_data[i], o_data[i], dims, OpBox{}, comm, n );
                ++i;

            }

            // 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 = 8
            // 8 + 8 + 8 + 8 + 8 + 8 + 8 + 8 = 64
            // 64 + 64 ...                   = 512 


            for (auto d : o_data){
                for (auto e : d) {
                    
                    REQUIRE(e == int(std::pow(8, n+1)));
                    //REQUIRE(e == 8);
                }
            }

            std::swap(i_data, o_data);



        }



    }


    
    

    SECTION("Star operation"){

        
        using comm_data_t = std::vector<int>;
        using comm_t = HpxMdCommunicator<comm_data_t, 2, ConnectivityType::Star>;
        std::string comm_name = "2by2_stencil_star";

        size_t num_local_partitions = 3;
        size_t num_localities = hpx::get_num_localities(hpx::launch::sync);
        size_t n_partitions = num_local_partitions * num_localities;
        size_t rank = hpx::get_locality_id();

        dimension<2> global_dims = {10, 10};
        std::array<bool, 2> periodicity  = {true, true};

        
        Decomposition<2> dec(
            global_dims,
            n_partitions,
            periodicity
        );

        CHECK(dec.global_grid_dimensions().elementwise_product() != size_t(0));

        std::vector<comm_t> comms;
        std::vector<comm_data_t> i_data;
        std::vector<comm_data_t> o_data;


        for (size_t i = 0; i < num_local_partitions; ++i){

            idx_t id = idx_t(rank * num_local_partitions + i);
            comms.push_back(
                comm_t(comm_name, id, dec)
            );

            auto local_dims = dec.local_grid_dimensions(id);
            i_data.push_back(comm_data_t(local_dims.elementwise_product(), 1));
            o_data.push_back(comm_data_t(local_dims.elementwise_product(), 0));

            call_sets(i_data[i], local_dims, OpStar{}, comms[i], size_t(0));
        }



        size_t n_steps = 10;

        for (size_t n = 0; n < n_steps; ++n){
            size_t i = 0;
            for (auto comm : comms) {

                auto dims = comm.get_decomposition().local_grid_dimensions(comm.id());
                apply_stencil(i_data[i], o_data[i], dims, OpStar{}, comm, n );
                ++i;

            }


            for (auto d : o_data){
                for (auto e : d) {
                    REQUIRE(e == int(std::pow(4, n+1)));
                }
            }

            std::swap(i_data, o_data);

        }


    }

    SECTION("SHORT TEST") {



        using comm_data_t = std::vector<int>;
        using comm_t = HpxMdCommunicator<comm_data_t, 2, ConnectivityType::Star>;
        std::string comm_name = "2by2_stencil_star_second";

        

        dimension<2> global_dims = {10, 10};
        std::array<bool, 2> periodicity  = {true, true};

        comm_t comm(comm_name, global_dims, periodicity);

        auto local_dims = comm.get_local_grid_dimensions();

        std::vector<int> i_data(local_dims.elementwise_product(), 1);        
        std::vector<int> o_data(local_dims.elementwise_product(), 0);        

        call_sets(i_data, local_dims, OpStar{}, comm, size_t(0)); 



        size_t n_steps = 10;

        for (size_t n = 0; n < n_steps; ++n){


            apply_stencil(i_data, o_data, local_dims, OpStar{}, comm, n);


            for (auto e : o_data){
                REQUIRE(e == int(std::pow(4, n+1)));
            }

            std::swap(i_data, o_data);

        }



    }

    


}
*/

TEST_CASE("Test op_to_iterators"){

    /*
    using namespace JADA;


    size_t nx = 2;
    size_t ny = 2;

    std::vector<position<2>> positions = {position<2> }
    */



}

TEST_CASE("TEST Pvec iterator set"){

    using namespace JADA;

    SECTION("Test 1"){

        size_t nx = 3;
        size_t ny = 3;

        dimension<2> dims{ny, nx};

        auto data = make_partitioned_vector<2, int>(dims);

        PositionSet<2> positions(
                                    {   
                                        {1, 0}, 
                                        {-1, 0},
                                        {0, 1},
                                        {0, -1},
                                        {0, 0}
                                    }
                                );

        auto center = data.begin() + 4;

        auto iters = positions_to_iterators(positions, center, dims);

        CHECK(iters.size() == positions.size());

        
        for (auto iter : iters) {
            *iter = 1;
        }

        /*hpx::partitioned_vector<int> correct{
            0,1,0,
            1,1,1,
            0,1,0
        };

        CHECK(data == correct);
        */

       //for (auto d : data){
        //   std::cout << d << std::endl;
       //}


    }


    SECTION("ASDfd"){

        auto min_max = find_min_max(TestOp{});

        CHECK(min_max.first == position<2>{-1, -1});
        CHECK(min_max.second == position<2>{1,1});

    }

    SECTION("Test segment_loop_bounds"){

        size_t nx = 4;
        size_t ny = 6;
        dimension<2> dims = {ny, nx};

        auto [begin, end] = segment_loop_bounds(dims, TestOp{});

        CHECK(begin == position<2>{1, 1});
        CHECK(end == position<2>{5, 2});

        



    }



}



TEST_CASE("Test stencil pvec"){


    using namespace JADA;

    SECTION("For vector"){


        size_t nx = 10;
        size_t ny = 11;

        dimension<2> dims{ny, nx};

        std::vector<int> in(dims.elementwise_product());
        std::fill(in.begin(), in.end(), 1);

        auto out = apply_stencil(in, dims, TestOp{});


        auto view = MdView(dims, out);

        CHECK(view[{0,0}] == 0);
        CHECK(view[{1,1}] == 4);

        for (auto pos : md_indices(position<2>{1,1}, position<2>{10, 9})) {
            REQUIRE(view[pos] == 4);
        }


        //view.pretty_print();
        
    }

    SECTION("For pvector") {


        size_t nx = 10;
        size_t ny = 11;

        dimension<2> dims{ny, nx};

        auto in = make_partitioned_vector<2, int>(dims);

        std::fill(in.begin(), in.end(), 1);

        CHECK(in[0] == 1);


        auto out = apply_stencil(in, dims, TestOp{});

        
        CHECK(out[0] == 0);

        for (auto pos : md_indices(position<2>{1,1}, position<2>{10, 9})) {

            auto idx = flatten<2, StorageOrder::RowMajor>(dims, pos);
            CHECK(out[size_t(idx)] == 4);

        }



        

    }

}