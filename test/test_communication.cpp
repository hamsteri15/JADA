#include "catch.hpp"

//#include "communication/communicator.hpp"
//#include "communication/mock_md_communicator.hpp"
#include "communication/md_communicator_base.hpp"
#include "communication/hpx_md_communicator.hpp"


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


/*
TEST_CASE("Test HpxMdCommunicator") {

    using namespace JADA;

    SECTION("Constructors") {

        REQUIRE_NOTHROW(HpxMdCommunicator<2, double>());
        REQUIRE_NOTHROW(HpxMdCommunicator<2, std::vector<double>>());

        Decomposition<2> dec({10, 10}, {3,3}, {false, false});

        REQUIRE_NOTHROW(HpxMdCommunicator<2, double>(0, dec));


    }


    





//    CHECK(1 == 2);
}
*/
