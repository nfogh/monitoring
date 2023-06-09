#include "utils.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include <monitoring/conditions/paramindex.hpp>
#include <monitoring/conditions/equals.hpp>

using namespace Monitoring;

TEST_CASE("ParamIndex", "[paramindex]")
{
  SECTION("scalar")
  {
    auto param1 = GenerateInterestingInts();
    auto param2 = GenerateInterestingInts();
    auto param3 = GenerateInterestingInts();
    auto param4 = GenerateInterestingInts();
    auto param5 = GenerateInterestingInts();

    REQUIRE(FirstParam(Equals(param1))(param1, param2, param3, param4, param5));
    REQUIRE(SecondParam(Equals(param2))(param1, param2, param3, param4, param5));
    REQUIRE(ThirdParam(Equals(param3))(param1, param2, param3, param4, param5));
    REQUIRE(FourthParam(Equals(param4))(param1, param2, param3, param4, param5));
    REQUIRE(ParamIndex<4>(Equals(param5))(param1, param2, param3, param4, param5));
  }
}
