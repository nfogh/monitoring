#include "utils.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include <monitoring/conditions/equals.hpp>

using namespace Monitoring;

TEST_CASE("Equals", "[equals]")
{
  SECTION("scalar")
  {
    auto lhs = GenerateInterestingInts();
    auto rhs = GenerateInterestingInts();

    REQUIRE(Equals(lhs)(rhs) == (lhs == rhs));
  }

  /*SECTION("array")
  {
    auto match1 = GenerateInterestingInts();
    auto match2 = GenerateInterestingInts();
    auto val = GenerateInterestingInts();
    REQUIRE(Equals({ match1, match2 })(val) == (match1 == val || match2 == val));
  }*/
}
