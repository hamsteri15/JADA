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
#include "communication/md_communicator.hpp"
#include "communication/hpx_md_communicator.hpp"
#include "grid/neighbours.hpp"
#include "stencil/apply_stencil.hpp"

#include "ops.hpp"


TEST_CASE("Test apply_stencil_parallel"){

    using namespace JADA;


    
    SECTION("Box operation"){

        
        using comm_data_t = std::vector<int>;
        using comm_t = HpxMdCommunicator<comm_data_t, 2, ConnectivityType::Box>;
        std::string comm_name = "2by2_stencil_box";

        size_t num_local_partitions = 1;
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

        }


        size_t i = 0;
        for (auto comm : comms) {

            auto dims = comm.get_decomposition().local_grid_dimensions(comm.id());
            apply_stencil(i_data[i], o_data[i], dims, OpBox{}, comm );
            ++i;

        }


        for (auto d : o_data){
            for (auto e : d) {
                REQUIRE(e == 8);
            }
        }

    }

    

    SECTION("Star operation"){

        
        using comm_data_t = std::vector<int>;
        using comm_t = HpxMdCommunicator<comm_data_t, 2, ConnectivityType::Star>;
        std::string comm_name = "2by2_stencil_star";

        size_t num_local_partitions = 1;
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

        }


        size_t i = 0;
        for (auto comm : comms) {

            auto dims = comm.get_decomposition().local_grid_dimensions(comm.id());
            apply_stencil(i_data[i], o_data[i], dims, OpStar{}, comm );
            ++i;

        }


        for (auto d : o_data){
            for (auto e : d) {
                REQUIRE(e == 4);
            }
        }

    }
    

}

