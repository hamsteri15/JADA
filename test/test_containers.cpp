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

    SECTION("Constructors") {

        REQUIRE_NOTHROW(StructuredData<2, double>());
        REQUIRE_NOTHROW(StructuredData<2, int>({10, 10}, {2,2}));
    }


    SECTION("Index access") {
        
        size_t nj = 4;
        size_t ni = 5;
        size_t ngc_j = 1;
        size_t ngc_i = 2;
        
//        size_t NJ = nj + 2*ngc_j;
//        size_t NI = ni + 2*ngc_i;

        dimension<2> dims = {nj, ni};
        dimension<2> padding = {ngc_j, ngc_i};


        StructuredData<2, int> d(dims, padding);

        REQUIRE_NOTHROW(d.access_any({-1,1}));
        REQUIRE_NOTHROW(d.access_any({-1,-1}));
        REQUIRE_NOTHROW(d.access_any({-idx_t(ngc_j),-idx_t(ngc_i)}));
        REQUIRE_NOTHROW(d.access_any({0,0}));
        REQUIRE_NOTHROW(d.access_any({idx_t(nj + ngc_j - 1), 0}));
        REQUIRE_NOTHROW(d.access_any({0, idx_t(ni + ngc_i - 1)}));



        auto begin = position<2>{0,0};
        auto end   = position<2>{dims + padding*2};
        
        int idx = 0;
        for (auto pos : md_indices(begin, end)){

            auto s_pos = pos - position<2>(padding);
            d.access_any(s_pos) = idx;
            ++idx;
        }
        
        CHECK(
            d.access_any({0,0}) == int(ngc_j * (ni + 2*ngc_i) + ngc_i)
        );

        /*
        CHECK(
            d.access_any({idx_t(nj - 1), idx_t(ni - 1)}) == int((nj + ngc_j) * ((ni + 2*ngc_i)) - ngc_i)
        );*/


    }



    //d.get_combined().pretty_print();



    /*






    StructuredData<2, int> d({8,9}, {2,2});

    REQUIRE_NOTHROW(d.access_any({0, -1}));


    d.access_any({0,0}) = 3;
    CHECK(d.access_any({0,0}) == 3);

    d.access_any({-1,0}) = 4;
    CHECK(d.access_any({-1,0}) == 4);

    d.access_any({-2,0}) = 4;
    CHECK(d.access_any({-2,0}) == 4);

    d.access_any({-1,-1}) = 4;
    CHECK(d.access_any({-1,-1}) == 4);

    d.access_any({-2,-2}) = 4;
    CHECK(d.access_any({-2,-2}) == 4);







    Neighbours<2, ConnectivityType::Box> n;

    int i = 1;
    for (auto dir : n.get()){

        auto copy = d.get_halo(dir);
        copy.set_all(i);
        d.put_halo(copy, dir);
        ++i;
    }

    auto combined = d.get_combined();

    combined.pretty_print();    

    d.set_all(1);

    auto copy = d;

    for (auto pos : md_indices(position<2>{0,0}, position<2>{8,9})){

        d.access_any(pos) = copy.access_any(pos + position<2>{-1,0})
                          + copy.access_any(pos + position<2>{ 1,0})
                          + copy.access_any(pos + position<2>{ 0,-1})
                          + copy.access_any(pos + position<2>{ 0, 1});
    }


    auto interior = d.get_interior();    


    for (size_t idx = 0; idx < 8*9; ++idx){
        REQUIRE(interior.data()[idx] == 4);
    }

    */




}