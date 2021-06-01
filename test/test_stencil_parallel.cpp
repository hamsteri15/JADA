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
#include "containers/partitioned_vector.hpp"
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

TEST_CASE("Test stencil pvec"){


    using namespace JADA;

    SECTION("For vector"){


        size_t nx = 10;
        size_t ny = 11;

        dimension<2> dims{ny, nx};

        std::vector<int> in(dims.elementwise_product());
        std::vector<int> out(dims.elementwise_product());

        std::fill(in.begin(), in.end(), 1);


        auto i_begin = in.begin() + 1 + idx_t(nx);
        auto i_end   = i_begin + idx_t(nx) - 2;

        auto o_begin = out.begin() + 1 + idx_t(nx);


        do_work_segment(i_begin, i_end, o_begin, dims, OpStar{});

        auto view = MdView(dimension<2>{ny, nx}, out);

        view.pretty_print();

    }

    SECTION("For pvector") {

        size_t nx = 4;
        size_t ny = 12;

        dimension<2> dims{ny, nx};

        auto v = make_partitioned_vector<2, int>(dims);
    
        std::iota(v.begin(), v.end(), 0);

        for (size_t i = 0; i < v.size(); ++i){
            std::cout << "Partition: "<<v.get_partition(i) << std::endl;
        }








        auto local_sbegin = v.segment_begin(hpx::get_locality_id());
        auto local_send = v.segment_end(hpx::get_locality_id());

        size_t count = local_segment_count(v);
        size_t s0 = local_first_segment_number(v);
        size_t s1 = local_last_segment_number(v);

        CHECK(count == size_t(std::distance(local_sbegin, local_send)));
        CHECK(count == s1 - s0);


        //std::cout << count << std::endl;
        //std::cout << first_segment_number << std::endl;

//        auto global_sbegin = v.segment_begin();

        //auto n = std::distance(global_sbegin, local_sbegin);


        /*for (auto it = local_sbegin; it != local_send; ++it){

            auto number = std::distance(global_sbegin, it);
            std::cout << number << std::endl;


        }
*/
//        std::fill(s_begin, s_end, 1);



        /*
        
        auto n_segs = segment_count(v);

        for (size_t i = 0; i < n_segs; ++i) {

            auto [begin, end] = get_segment(v, i);

            

        } 

        */
        /*



        auto s_begin = v.segment_begin(hpx::get_locality_id());
        auto s_end = v.segment_end()
        */

        //auto s_begin = v.segment_begin(hpx::get_locality_id());
        //auto s_end = v.segment_end(hpx::get_locality_id());

        //std::cout <


//        for (auto s_it = v.segment_begin(hpx::get_locality_id())

        /*
        v.share


        for (auto it = v.begin(); it != v.end(); ++it){

            *it = int(hpx::get_locality_id());

        }


        auto localities = hpx::find_all_localities();

        hpx::cout << "My locality id: " << hpx::get_locality_id() << " Locality count: " << localities.size() << hpx::endl;

        if (hpx::get_locality_id() == 0) {
        
            for (auto e : v) {
                std::cout << e << std::endl;
            }
        }
        */
        /*
        size_t nx = 10;
        size_t ny = 11;

        dimension<2> dims{ny, nx};

        auto in = make_partitioned_vector<2, int>(dims);
        auto out = make_partitioned_vector<2, int>(dims);

        std::fill(in.begin(), in.end(), 1);


        auto i_begin = in.begin() + 1 + idx_t(nx);
        auto i_end   = i_begin + idx_t(nx) - 2;

        auto o_begin = out.begin() + 1 + idx_t(nx);

        do_work_segment(i_begin, i_end, o_begin, dims, OpStar{});

        for (auto o : out) {
            std::cout << o << std::endl;
        }
        */


    }

}