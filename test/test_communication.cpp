#include "catch.hpp"

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
#include <vector>

using vector_t = std::vector<double>;
HPX_REGISTER_CHANNEL_DECLARATION(vector_t)
HPX_REGISTER_CHANNEL(vector_t, TEST_COMM_CHANNEL)


using vector_t2 = std::vector<int>;
HPX_REGISTER_CHANNEL_DECLARATION(vector_t2)
HPX_REGISTER_CHANNEL(vector_t2, TEST_COMM_CHANNEL2)


//using own_communicator_type = communicator_own<communication_type>;


TEST_CASE("Test MdCommunicatorBase"){

    using namespace JADA;





    SECTION("Constructors") {

        REQUIRE_NOTHROW(MdCommunicator<2, ConnectivityType::Star>());
        
        REQUIRE_NOTHROW(MdCommunicator<2, ConnectivityType::Star>(
            0,
            Decomposition<2>({10,10}, {3,3}, {false, false}) 
        ));
        REQUIRE_THROWS(MdCommunicator<2, ConnectivityType::Star>(
            0,
            Decomposition<2>({10, 10}, {11, 3}, {false, false})
        ));
    }

    SECTION("Public member functions") {


        MdCommunicator<2, ConnectivityType::Star> comm(
            0,
            Decomposition<2>({10, 10}, {4,4}, {false, false})
        );


        CHECK(comm.has_neighbour({0,1}));
        CHECK(!comm.has_neighbour({-1, 0}));

    }


}



TEST_CASE("Test HpxMdCommunicator") {


    using namespace JADA;



    SECTION("Constructors"){
    
        //using Comm = communicator_t<2, ConnectivityType::Box>;

        REQUIRE_NOTHROW(HpxMdCommunicator<std::vector<int>, 2, ConnectivityType::Star>());
        REQUIRE_NOTHROW(HpxMdCommunicator<std::vector<int>, 2, ConnectivityType::Box>());
        REQUIRE_NOTHROW(HpxMdCommunicator<std::vector<double>, 2, ConnectivityType::Box>());

        REQUIRE_NOTHROW(HpxMdCommunicator<std::vector<double>, 2, ConnectivityType::Box>(
            0,
            Decomposition<2>({11,12}, 2, {false, false})
        ));

        REQUIRE_NOTHROW(HpxMdCommunicator<std::vector<int>, 2, ConnectivityType::Star>(
            1,
            Decomposition<2>({11,12}, 2, {false, false})
        ));


        //REQUIRE_NOTHROW(communicator_t());

    }

    SECTION("RENAME"){

        /*
        
        dimension<2> dims = {15, 12};
        size_t n_parts = 3;
        std::array<bool, 2>  periodicity{false, false};


        using Comm = HpxMdCommunicator<std::vector<int>, 2, ConnectivityType::Star>;

        Decomposition<2> dec(dims, n_parts, periodicity);

        std::vector<Comm> comms;

        for (size_t i = 0; i < n_parts; ++i){

            comms.push_back(Comm(idx_t(i), dec));
        }
        

        for (auto& comm : comms) {

            for (auto dir : comm.get_directions()){
                std::vector<int> data(3, int(comm.id()));
                comm.set(dir, std::move(data), 0);
            }

        }


        
        for (auto& comm : comms) {

            for (auto dir : comm.get_directions()){
                auto data = comm.get(dir, 0).get();
                std::cout << data[0] << std::endl;
                
            }
        }
        */



    }


    /*

    for (size_t i = 0; i < n_parts; ++i) {

    }
    */











//    CHECK(1 == 2);
}
