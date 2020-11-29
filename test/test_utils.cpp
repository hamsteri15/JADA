#include <iostream>
#include "catch.hpp"
#include "utils/math_vector.hpp"

TEST_CASE("Test math vector"){

    using namespace JADA::Utils;

    SECTION("Constructors"){

        MathVector<int,2> v1 = {1,2};
        auto v2 = v1;
        MathVector<int,2> v3(v1);
        MathVector<int,2> v4;

        REQUIRE_THROWS(MathVector<int,2>{1,2,3});

        CHECK(v1[0] == 1);
        CHECK(v2[0] == 1);
        CHECK(v3[0] == 1);
        CHECK(v4[0] == 0);
        CHECK(v4[1] == 0);


    }

    SECTION("Operations"){
        MathVector<double ,3> v;

        MathVector<int, 2> a = {1,2};
        MathVector<int, 2> b = {3,4};


        auto c = a * 3 + b - a / 3;

        CHECK(c[0] == 1 * 3 + 3 - 1 / 3);


        CHECK(a[0] == 1);


        CHECK(a != b);
        a = c;
        CHECK(a == c);
        CHECK(a[0] == c[0]);
        CHECK(a[1] == c[1]);

        for (auto& elem : a){
            elem = 43;
        }
        CHECK(a[0] == 43);
        CHECK(a[1] == 43);

        CHECK(std::accumulate(std::begin(a), std::end(a), 0) == 2*43);
        CHECK(std::accumulate(std::cbegin(a), std::cend(a), 0) == 2*43);
        CHECK(std::accumulate(std::rbegin(a), std::rend(a), 0) == 2*43);
        CHECK(std::accumulate(std::crbegin(a), std::crend(a), 0) == 2*43);


        a[0] = 1;
        CHECK(a.min() == 1);
        CHECK(a.max() == 43);
        a[0] = -2;
        a[1] = -2;
        CHECK(a.abs() == MathVector<int, 2>{2,2});

    }




}
