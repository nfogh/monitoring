#include "utils.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <limits>
#include <monitoring/conditions/between.hpp>

using namespace Monitoring;

namespace {
constexpr int _ = 0;
}

TEST_CASE("Between", "[between]")
{
  SECTION("Between returns true for integers")
  {
    auto lhs = GenerateInterestingInts();
    auto rhs = GenerateInterestingInts();
    auto mid = GenerateInterestingInts();

    REQUIRE(Between(lhs, rhs)(_, mid) == (lhs <= mid && mid <= rhs));
  }

  SECTION("Between returns true for floats")
  {
    auto lhs = GenerateInterestingFloats();
    auto rhs = GenerateInterestingFloats();
    auto mid = GenerateInterestingFloats();

    REQUIRE(Between(lhs, rhs)(_, mid) == (lhs <= mid && mid <= rhs));
  }
}

TEST_CASE("BetweenGetter", "[between]")
{

  SECTION("Between with a getter returns true for integers")
  {
    auto lhs = GENERATE(
      Data<int>(std::numeric_limits<int>::min()), Data(-1), Data(0), Data(1), Data(std::numeric_limits<int>::max()));
    auto rhs = GENERATE(
      Data<int>(std::numeric_limits<int>::min()), Data(-1), Data(0), Data(1), Data(std::numeric_limits<int>::max()));
    auto middle = GENERATE(
      Data<int>(std::numeric_limits<int>::min()), Data(-1), Data(0), Data(1), Data(std::numeric_limits<int>::max()));

    REQUIRE(Between(Val<int>(), lhs, rhs)(_, middle) == (lhs.val <= middle.val && middle.val <= rhs.val));
  }

  SECTION("Between returns true for floats")
  {
    auto lhs = GENERATE(Data(std::numeric_limits<double>::lowest()),
      Data(-std::numeric_limits<double>::min()),
      Data(0.0),
      Data(std::numeric_limits<double>::min()),
      Data(std::numeric_limits<double>::max()));
    auto rhs = GENERATE(Data(std::numeric_limits<double>::lowest()),
      Data(-std::numeric_limits<double>::min()),
      Data(0.0),
      Data(std::numeric_limits<double>::min()),
      Data(std::numeric_limits<double>::max()));
    auto middle = GENERATE(Data(std::numeric_limits<double>::lowest()),
      Data(-std::numeric_limits<double>::min()),
      Data(0.0),
      Data(std::numeric_limits<double>::min()),
      Data(std::numeric_limits<double>::max()));

    REQUIRE(Between(Val<double>(), lhs, rhs)(_, middle) == (lhs.val <= middle.val && middle.val <= rhs.val));
  }
}
