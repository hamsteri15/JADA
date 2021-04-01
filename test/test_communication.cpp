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

using comm_data_t = std::vector<int>;
HPX_REGISTER_CHANNEL_DECLARATION(comm_data_t)
HPX_REGISTER_CHANNEL(comm_data_t, TEST_COMM_CHANNEL)

using namespace JADA;



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
    
        /*
        REQUIRE_NOTHROW(HpxMdCommunicator<comm_data_t, 2, ConnectivityType::Box>());

        REQUIRE_NOTHROW(HpxMdCommunicator<comm_data_t, 2, ConnectivityType::Box>(
            0,
            Decomposition<2>({11,12}, 2, {false, false})
        ));
        */

    }

    
    /*
    SECTION("1by1 periodic star communication"){

        using comm_t = HpxMdCommunicator<comm_data_t, 2, ConnectivityType::Star>;


        Decomposition<2> dec(
            {10, 10},
            {1, 1},
            {true, true}
        );

        std::vector<comm_t> comms = 
        {
            comm_t(0, dec),
        };

        auto all_dirs = comms[0].get_directions();

        
        for (auto& comm : comms){

            for (direction<2> dir : all_dirs) {
                if (comm.has_neighbour(dir)){
                    std::cout << dir << std::endl;


                    comm_data_t data(1, int(comm.id()));
                    comm.set(dir, std::move(data), 0);
                }
            }

        }


        
        CHECK(comms[0].get({0, 1}, 0).get().front() == 0);
        CHECK(comms[0].get({1, 0}, 0).get().front() == 0);
        
        CHECK(comms[0].get({-1, 0}, 0).get().front() == 0);
        CHECK(comms[0].get({0, -1}, 0).get().front() == 0);
        

    }
    */

    
    
    SECTION("2by2 star communication"){

        using comm_t = HpxMdCommunicator<comm_data_t, 2, ConnectivityType::Star>;


        Decomposition<2> dec(
            {10, 10},
            {2,2},
            {false, false}
        );

        std::vector<comm_t> comms = 
        {
            comm_t(0, dec),
            comm_t(1, dec),
            comm_t(2, dec),
            comm_t(3, dec)
        };

        auto all_dirs = comms[0].get_directions();

        
        for (auto& comm : comms){

            for (direction<2> dir : all_dirs) {
                if (comm.has_neighbour(dir)){
                    comm_data_t data(1, int(comm.id()));
                    comm.set(dir, std::move(data), 0);
                }
            }

        }

        // topology is like this
        //  ---- i 
        //  |       0, 1
        //  |       2, 3  
        //  j

        CHECK(comms[0].get({0, 1}, 0).get().front() == 1);
        CHECK(comms[0].get({1, 0}, 0).get().front() == 2);

        CHECK(comms[1].get({0, -1}, 0).get().front() == 0);
        CHECK(comms[1].get({1, 0}, 0).get().front() == 3);


        CHECK(comms[2].get({0, 1}, 0).get().front() == 3);
        CHECK(comms[2].get({-1, 0}, 0).get().front() == 0);
        
        CHECK(comms[3].get({0, -1}, 0).get().front() == 2);
        CHECK(comms[3].get({-1, 0}, 0).get().front() == 1);

    }

    
    /*
    SECTION("2by2 periodic box communication"){

        using comm_t = HpxMdCommunicator<comm_data_t, 2, ConnectivityType::Box>;


        Decomposition<2> dec(
            {10, 10},
            {2,2},
            {true, true}
        );

        std::vector<comm_t> comms = 
        {
            comm_t(0, dec),
            comm_t(1, dec),
            comm_t(2, dec),
            comm_t(3, dec)
        };

        auto all_dirs = comms[0].get_directions();

        
        for (auto& comm : comms){

            for (direction<2> dir : all_dirs) {
                if (comm.has_neighbour(dir)){
                    comm_data_t data(1, int(comm.id()));
                    comm.set(dir, std::move(data), 0);
                }
            }

        }

        // topology is like this
        //  ---- i 
        //  |       0, 1
        //  |       2, 3  
        //  j
        
        for (auto& comm : comms){

            for (direction<2> dir : all_dirs) {
                if (comm.has_neighbour(dir)){
                    auto data = comm.get(dir, 0).get();
                    std::cout << "Id " << comm.id() << " got " << data.front() << " from " << dir << std::endl;
                }
            }

        }
        
        //CHECK(comms[0].get({0, 1}, 0).get().front() == 1);
//        CHECK(comms[0].get({0, -1}, 0).get().front() == 1);

        //CHECK(comms[0].get({1, 0}, 0).get().front() == 2);
        //CHECK(comms[0].get({-1, 0}, 0).get().front() == 2);

    }

    */
    







//    CHECK(1 == 2);
}
