#include "utils.h"
#include <monitoring/conditions/equals.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>

using namespace Monitoring;

TEST_CASE("Equals", "[equals]")
{
  SECTION("Equals")
  {
    auto someInt = GenerateInterestingInts();
    auto someOtherInt = GenerateInterestingInts();
    REQUIRE(Equals(someInt)(someOtherInt) == (someInt == someOtherInt));
  }
}
