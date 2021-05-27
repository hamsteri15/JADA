#include "catch.hpp"

#include "concepts/iterable.hpp"
#include "concepts/indexable.hpp"

struct Fail{};



TEST_CASE("Test Iterable"){

    using namespace JADA;

    auto f = []<Iterable T>(T& vec) { 
        for (auto& v : vec) {v = 1;}
     };
    std::vector<int> v(1);


    f(v);

    CHECK(v[0] == 1);

}

TEST_CASE("Test Indexable"){

    using namespace JADA;

    auto f = []<Indexable T>(T& vec) { 
        vec[size_t(0)] = 0;
        vec[size_t(1)] = 1;
     };
    std::vector<int> v(1);


    f(v);

    CHECK(v[0] == 0);
    CHECK(v[1] == 1);

}