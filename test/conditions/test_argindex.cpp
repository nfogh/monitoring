#include "utils.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <monitoring/conditions/argindex.hpp>
#include <monitoring/conditions/equals.hpp>

using namespace Monitoring;

TEST_CASE("ArgIndex", "[argindex]")
{
  auto checker = DefaultConditionChecker();

  SECTION("scalar")
  {
    auto arg0 = GenerateInterestingInts();
    auto arg1 = GenerateInterestingInts();
    auto arg2 = GenerateInterestingInts();
    auto arg3 = GenerateInterestingInts();
    auto arg4 = GenerateInterestingInts();

    REQUIRE(checker(FirstArg(Equals(arg0)), arg0, arg1, arg2, arg3, arg4));
    REQUIRE(checker(SecondArg(Equals(arg1)), arg0, arg1, arg2, arg3, arg4));
    REQUIRE(checker(ThirdArg(Equals(arg2)), arg0, arg1, arg2, arg3, arg4));
    REQUIRE(checker(FourthArg(Equals(arg3)), arg0, arg1, arg2, arg3, arg4));
    REQUIRE(checker(Args<4>(Equals(arg4)), arg0, arg1, arg2, arg3, arg4));

    // These do the same, but one is a lambda and the other is a composition of FirstArg and SecondArg
    // to test that FirstArg and SecondArg can be used within Args.
    REQUIRE(checker(Args<0, 1>([&](auto a, auto b) { return a == arg0 && b == arg1; }), arg0, arg1, arg2, arg3, arg4));
    REQUIRE(checker(Args<0, 1>(FirstArg(Equals(arg0)) && SecondArg(Equals(arg1))), arg0, arg1, arg2, arg3, arg4));

    REQUIRE(checker(Args<2, 4>(FirstArg(Equals(arg2)) && SecondArg(Equals(arg4))), arg0, arg1, arg2, arg3, arg4));
    REQUIRE(checker(Args<2, 4>([&](auto a, auto b) { return a == arg2 && b == arg4; }), arg0, arg1, arg2, arg3, arg4));
  }
}
