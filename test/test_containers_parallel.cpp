#include "catch.hpp"
#include "containers/distributed_data.hpp"


//REGISTER_PARTITION_SERVER(2, double);
//REGISTER_PARTITION_SERVER(2, int);


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