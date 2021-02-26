#include "catch.hpp"

//#include "communication/communicator.hpp"
//#include "communication/mock_md_communicator.hpp"
#include "communication/md_communicator_base.hpp"
#include "communication/hpx_md_communicator.hpp"
#include <hpx/include/components.hpp>
#include <vector>



typedef std::vector<double> vector_t;

HPX_REGISTER_CHANNEL_DECLARATION(vector_t)
HPX_REGISTER_CHANNEL(vector_t, stencil_communication)

TEST_CASE("Test MdCommunicatorBase"){

    using namespace JADA;



    SECTION("Constructors") {

        REQUIRE_NOTHROW(MdCommunicatorBase<2, ConnectivityType::Star>());
        
        REQUIRE_NOTHROW(MdCommunicatorBase<2, ConnectivityType::Star>(
            0,
            Decomposition<2>({10,10}, {3,3}, {false, false}) 
        ));
        REQUIRE_THROWS(MdCommunicatorBase<2, ConnectivityType::Star>(
            0,
            Decomposition<2>({10, 10}, {11, 3}, {false, false})
        ));
    }


    SECTION("Public member functions") {


        MdCommunicatorBase<2, ConnectivityType::Star> comm(
            0,
            Decomposition<2>({10, 10}, {4,4}, {false, false})
        );


        CHECK(comm.has_neighbour({0,1}));
        CHECK(!comm.has_neighbour({-1, 0}));

        CHECK(comm.get_neighbour_ids() == 
            std::vector<idx_t>{1, 4, NEIGHBOUR_ID_NULL, NEIGHBOUR_ID_NULL}
        );




    }



}


TEST_CASE("Test HpxMdCommunicator") {

    using namespace JADA;

    SECTION("Constructors") {

        REQUIRE_NOTHROW(HpxMdCommunicator<2, double>());
        
        REQUIRE_NOTHROW(HpxMdCommunicator<2, vector_t>());


        size_t n_domains = 12;

        

        REQUIRE_NOTHROW(
            HpxMdCommunicator<2, vector_t>(
                idx_t(hpx::get_locality_id()),
                Decomposition<2>({100, 100}, n_domains, {true, true})
            )
        );




                

    }

    SECTION("set/get") {

        
        /*
        size_t n_domains = hpx::get_num_localities(hpx::launch::sync);

        std::cout << "Domain count: " << n_domains << std::endl; 

        //std::size_t num_localities = hpx::get_num_localities(hpx::launch::sync);

        HpxMdCommunicator<2, vector_t> comm(
                idx_t(hpx::get_locality_id()),
                Decomposition<2>({100, 100}, n_domains, {true, true})
            );

        position<2> dir = {1,0};

        size_t tag = 0;

        comm.set(dir, std::vector<double>(11), tag);

        
        std::vector<double> from = comm.get(-dir, tag).get();


        for (auto f : from) {
            std::cout << f << std::endl;
        }
        */

    }

    





//    CHECK(1 == 2);
}
