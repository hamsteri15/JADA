#include "catch.hpp"
#include "containers/soa.hpp"
#include "containers/md_array.hpp"
#include "containers/md_view.hpp"
#include "containers/structured_data.hpp"
#include "loops/md_index_loops.hpp"

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

    MdArray<2, int> b({2,3});

    b.set_all(1);

    for (size_t i = 0; i < 2 * 3; ++i) {
        REQUIRE(b.data()[i] == 1);
    }


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



    StructuredData<2, int> d({8,9}, {2,2});

    REQUIRE_NOTHROW(d.access_any({0, -1}));


    d.access_any({0,0}) = 3.0;
    CHECK(d.access_any({0,0}) == 3.0);


    d.set_all(1);

    auto copy = d;

    for (auto pos : md_indices(position<2>{0,0}, position<2>{8,9})){

        d.access_any(pos) = copy.access_any(pos + position<2>{-1,0})
                          + copy.access_any(pos + position<2>{ 1,0})
                          + copy.access_any(pos + position<2>{ 0,-1})
                          + copy.access_any(pos + position<2>{ 0, 1});
    }


    auto interior = d.get_interior();    


    for (size_t i = 0; i < 8*9; ++i){
        REQUIRE(interior.data()[i] == 4);
    }







}