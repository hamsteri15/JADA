#include "catch.hpp"
#include "containers/soa.hpp"
#include "containers/md_array.hpp"
#include "containers/md_view.hpp"
#include "containers/structured_data.hpp"


TEST_CASE("Test Soa"){

    using namespace JADA;

    SECTION("Constructors"){
        REQUIRE_NOTHROW(Soa());

    }

}




TEST_CASE("Test MdArray"){

    using namespace JADA;

    MdArray<2, double> a({10, 10});

    CHECK(a.size() == 100);    

    a[{1,1}] = 3.0;
    CHECK(a[{1,1}] == 3.0);

}



TEST_CASE("Test MdView"){

    using namespace JADA;

    std::vector<double> data(64);
    auto view = MdView(dimension<2>{8, 8}, data);

    view[{1,1}] = 3.0;
    CHECK(view[{1,1}] == 3.0);

}

TEST_CASE("Test StructuredData"){

    using namespace JADA;

    REQUIRE_NOTHROW(StructuredData<2, double>());
    REQUIRE_NOTHROW(StructuredData<2, int>({10, 10}, {2,2}));


    




}