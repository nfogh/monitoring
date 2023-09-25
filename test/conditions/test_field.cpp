#include "utils.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include <monitoring/conditions/field.hpp>
#include <monitoring/conditions/equals.hpp>

using namespace Monitoring;

TEST_CASE("Field", "[field]")
{
  SECTION("Test that we can extract fields from structs")
  {
    struct Data
    {
      int a = 0;
      int b = 1;
      int c = 2;
    } data;

    REQUIRE( Field(&Data::a, Equals(0))(data));
    REQUIRE(!Field(&Data::a, Equals(1))(data));

    REQUIRE( Field(&Data::b, Equals(1))(data));
    REQUIRE(!Field(&Data::b, Equals(0))(data));

    REQUIRE( Field(&Data::c, Equals(2))(data));
    REQUIRE(!Field(&Data::c, Equals(0))(data));
  }

}
