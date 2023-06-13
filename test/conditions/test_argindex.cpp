#include "utils.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include <monitoring/conditions/argindex.hpp>
#include <monitoring/conditions/equals.hpp>
#include <monitoring/conditions/difference.hpp>

using namespace Monitoring;

TEST_CASE("ArgIndex", "[argindex]")
{
  SECTION("scalar")
  {
    auto arg0 = GenerateInterestingInts();
    auto arg1 = GenerateInterestingInts();
    auto arg2 = GenerateInterestingInts();
    auto arg3 = GenerateInterestingInts();
    auto arg4 = GenerateInterestingInts();

    REQUIRE(FirstArg(Equals(arg0)) (arg0, arg1, arg2, arg3, arg4));
    REQUIRE(SecondArg(Equals(arg1))(arg0, arg1, arg2, arg3, arg4));
    REQUIRE(ThirdArg(Equals(arg2)) (arg0, arg1, arg2, arg3, arg4));
    REQUIRE(FourthArg(Equals(arg3))(arg0, arg1, arg2, arg3, arg4));
    REQUIRE(Args<4>(Equals(arg4))  (arg0, arg1, arg2, arg3, arg4));
    REQUIRE(Args<0, 1>(FirstArg(Equals(arg0)) && SecondArg(Equals(arg1)))(arg0, arg1, arg2, arg3, arg4));
    REQUIRE(Args<2, 4>(FirstArg(Equals(arg2)) && SecondArg(Equals(arg4)))(arg0, arg1, arg2, arg3, arg4));
  }
}
