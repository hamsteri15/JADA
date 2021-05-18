#include "catch.hpp"
#include "containers/distributed_data.hpp"
#include "containers/structured_data_view.hpp"
#include "communication/hpx_md_communicator.hpp"
#include "ops.hpp"
#include "stencil/apply_stencil.hpp"

//REGISTER_PARTITION_SERVER(2, double);
//REGISTER_PARTITION_SERVER(2, int);


TEST_CASE("Test StructuredDataView"){

    
    using namespace JADA;

    using comm_data_t = std::vector<int>;
        
    using comm_t = HpxMdCommunicator<comm_data_t, 2, ConnectivityType::Box>;
    std::string comm_name = "structuredDataView2by2";

    size_t num_local_partitions = 2;
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

        

    std::vector<comm_t> comms;
    std::vector<comm_data_t> i_data;
  
    for (size_t i = 0; i < num_local_partitions; ++i){

        idx_t id = idx_t(rank * num_local_partitions + i);
        comms.push_back(
            comm_t(comm_name, id, dec)
        );

        auto local_dims = dec.local_grid_dimensions(id);
        i_data.push_back(comm_data_t(local_dims.elementwise_product(), 1));
        call_sets(i_data[i], local_dims, OpBox{}, comms[i], size_t(0));

    }

    for (size_t i = 0; i < num_local_partitions; ++i){

        auto comm = comms[i];
        auto data = i_data[i];
        idx_t id = idx_t(rank * num_local_partitions + i);


        std::vector<direction<2>> dirs;
        std::vector<hpx::shared_future<comm_data_t>> datas;

        for (auto dir : comm.get_directions()){

            
            if (comm.has_neighbour(dir)) {
                dirs.push_back(dir);
                datas.push_back(comm.get_shared(dir, size_t(0)));

            }

        }

        auto l_dims = dec.local_grid_dimensions(id);
        dimension<2> padding = {1,1};
        auto view = StructuredDataView<2, int, ConnectivityType::Box>(l_dims, padding, i_data[i], datas, dirs);


        REQUIRE_NOTHROW(view.at({-1, 0}));
        REQUIRE_NOTHROW(view.at({-1, -1}));
        REQUIRE_NOTHROW(view.at({-1, 4}));
        


    }



}


TEST_CASE("Test partition_data"){

    using namespace JADA;

    SECTION("Constructors") {

        REQUIRE_NOTHROW(partition_data<2, int>(dimension<2>{3, 4}));
        REQUIRE_NOTHROW(partition_data<2, double>(dimension<2>{3, 4}));


    }


}





TEST_CASE("Test partition_server") {


    using namespace JADA;

    SECTION("Constructors") {

        REQUIRE_NOTHROW(partition_server<2, int>(dimension<2>{3, 4}));
        REQUIRE_NOTHROW(partition_server<2, double>(dimension<2>{3, 4}));


    }

    SECTION("get_data") {

        

        partition_server<2, int> part(dimension<2>{3,5});

        REQUIRE_NOTHROW(part.get_data(direction<2>{1,1}, dimension<2>{2,2}));

  



    }

}

