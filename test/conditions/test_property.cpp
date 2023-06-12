#include "utils.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include <monitoring/conditions/property.hpp>
#include <monitoring/conditions/equals.hpp>

using namespace Monitoring;

TEST_CASE("Property", "[property]")
{
  SECTION("Test that we can extract properties from structs")
  {
    struct Data
    {
        int GetA() const { return a; }
        int GetB() const { return b; }
        int GetC() const { return c; }
        int a = 0;
        int b = 1;
        int c = 2;
    } data;

    REQUIRE( Property(&Data::GetA, Equals(0))(data));
    REQUIRE(!Property(&Data::GetA, Equals(1))(data));

    REQUIRE( Property(&Data::GetB, Equals(1))(data));
    REQUIRE(!Property(&Data::GetB, Equals(0))(data));

    REQUIRE( Property(&Data::GetC, Equals(2))(data));
    REQUIRE(!Property(&Data::GetC, Equals(0))(data));
  }

}