#include "catch.hpp"
#include "containers/soa.hpp"
#include "containers/md_array.hpp"
#include "containers/md_view.hpp"
#include "containers/structured_data.hpp"
#include "containers/md_indexable.hpp"
#include "loops/md_index_loops.hpp"

TEST_CASE("Test Soa"){

    using namespace JADA;

    SECTION("Constructors"){
        REQUIRE_NOTHROW(Soa());

    }

}
namespace JADA {

template<size_t N>
struct Stuff : MdIndexable<double, N, Stuff<N>>{

   explicit Stuff(dimension<N> dim) : m_dim(dim), m_data(dim.elementwise_product()) {}


    constexpr const double* get_ptr() const { return m_data.data();}
    constexpr       double* get_ptr()       { return m_data.data(); }
    constexpr dimension<N> const get_dimension() const { return m_dim; }

private:
    dimension<N>        m_dim;
    std::vector<double> m_data;

};

}


TEST_CASE("Test MdIndexable") {

    using namespace JADA;


    Stuff<3> s(dimension<3>{3,4,5});

    REQUIRE_NOTHROW(s.data());


    //s(position<3>{1,3,3}) = 32.0;

//    CHECK(s({1,3,3}) == 32.0);



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
    auto                view = MdView(dimension<2>{8, 8}, data);

    view[{1, 1}] = 3.0;
    CHECK(view[{1, 1}] == 3.0);
    CHECK(data[9] == 3.0);
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
        
        dimension<2> dims = {nj, ni};
        dimension<2> padding = {ngc_j, ngc_i};


        StructuredData<2, int> d(dims, padding);

        d.at({0,0}) = 7;
        const int& test = d.at({0,0});
        CHECK(test == 7);

        REQUIRE_NOTHROW(d.at({-1,1}));
        REQUIRE_NOTHROW(d.at({-1,-1}));
        REQUIRE_NOTHROW(d.at({-idx_t(ngc_j),-idx_t(ngc_i)}));
        REQUIRE_NOTHROW(d.at({0,0}));
        REQUIRE_NOTHROW(d.at({idx_t(nj + ngc_j - 1), 0}));
        REQUIRE_NOTHROW(d.at({0, idx_t(ni + ngc_i - 1)}));

        REQUIRE_THROWS(d.at({-2, 0}));
        REQUIRE_THROWS(d.at({1, -3}));



        auto begin = position<2>{0,0};
        auto end   = position<2>{dims + padding*2};
        
        int idx = 0;
        for (auto pos : md_indices(begin, end)){

            auto s_pos = pos - position<2>(padding);
            d.at(s_pos) = idx;
            ++idx;
        }

        CHECK(d.at({0,0}) == 11);
        CHECK(d.at({1,1}) == 21);
        CHECK(d.at({idx_t(nj-1),idx_t(ni-1)}) == 42);

    }







}