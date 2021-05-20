#include "catch.hpp"

#include "operation/field.hpp"
#include "loops/dimension.hpp"

TEST_CASE("Test Field") {

    using namespace JADA;

    SECTION("Constructors") {

        REQUIRE_NOTHROW(Field<2, double>({3,4}));
        REQUIRE_NOTHROW(Field<2, double>({1,4}));

        /*
        Field<2, double> f(dimension<2>{3, 3});


        f[{1,2}] = 3.0;
        CHECK(f[{1,2}] == 3.0);
        */


    }

    SECTION("Math") {

        Field<2, double> a({3,3});
        Field<2, double> b({3,3});

        a = 1.0;
        b = 2.0;

        Field<2, double> c = a + b;

        CHECK(c[{1,1}] == 3.0);


    }



}