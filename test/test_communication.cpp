#include "catch.hpp"

//#include "communication/communicator.hpp"
//#include "communication/mock_md_communicator.hpp"
#include "communication/hpx_md_communicator.hpp"



TEST_CASE("Test HpxMdCommunicator") {

    using namespace JADA;

    SECTION("Constructors") {
        //REQUIRE_NOTHROW(MockMdCommunicator<2, std::vector<double>>());

        REQUIRE_NOTHROW(HpxMdCommunicator<2, double>());
        REQUIRE_NOTHROW(HpxMdCommunicator<2, std::vector<double>>());

        Decomposition<2> dec({10, 10}, {3,3}, {false, false});

        REQUIRE_NOTHROW(HpxMdCommunicator<2, double>(0, dec));


    }

    SECTION("has_neighbour") {


        HpxMdCommunicator<2, double> comm;
        //CHECK(comm.buffer_size() == 4);

//        HpxMdCommunicator<2, double> comm();
//        CHECK(comm.buffer_size() == 4); 

        //CHECK(comm.has_neighbour({1,0}));


        /*
        Decomposition<2> dec({10, 10}, {2,2}, {false, false});

        MockMdCommunicator<2, double> comm(0, dec);

        CHECK(comm.has_neighbour({0,1}));
        CHECK(!comm.has_neighbour({0,-1}));
        CHECK(comm.has_neighbour({1,1}));
        CHECK(!comm.has_neighbour({-1,1}));

        size_t tag = 4444;
        comm.set({0,1}, 1, tag);
        */
        /*
        REQUIRE_THROWS(comm.buffer_idx({0,0}));
        CHECK(comm.buffer_idx({0, 1}) == 0);
        CHECK(comm.buffer_idx({1, 0}) == 1);
        CHECK(comm.buffer_idx({-1, 0}) == 2);
        //CHECK(comm.buffer_idx({0, 0}) == 0);
        */

    }

    





//    CHECK(1 == 2);
}

