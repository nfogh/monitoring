#include "utils.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include <monitoring/conditions/equals.hpp>
#include <monitoring/conditions/property.hpp>

using namespace Monitoring;

TEST_CASE("Property", "[property]")
{
  auto checker = DefaultConditionChecker();

  SECTION("Test that we can extract properties from structs")
  {
    struct Data
    {
      [[nodiscard]] int GetA() const { return a; }
      [[nodiscard]] int GetB() const { return b; }
      [[nodiscard]] int GetC() const { return c; }
      int a = 0;
      int b = 1;
      int c = 2;
    } data;

    REQUIRE(checker(Property(&Data::GetA, Equals(0)), data));
    REQUIRE(checker(!Property(&Data::GetA, Equals(1)), data));

    REQUIRE(checker(Property(&Data::GetB, Equals(1)), data));
    REQUIRE(checker(!Property(&Data::GetB, Equals(0)), data));

    REQUIRE(checker(Property(&Data::GetC, Equals(2)), data));
    REQUIRE(checker(!Property(&Data::GetC, Equals(0)), data));
  }
}
