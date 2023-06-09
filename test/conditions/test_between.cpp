#include "utils.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <limits>
#include <monitoring/conditions/between.hpp>

using namespace Monitoring;

TEST_CASE("Between", "[between]")
{
  auto checker = DefaultConditionChecker();
  SECTION("Between returns true for integers")
  {
    auto lhs = GenerateInterestingInts();
    auto rhs = GenerateInterestingInts();
    auto mid = GenerateInterestingInts();

    REQUIRE(checker(Between(lhs, rhs), mid) == (lhs <= mid && mid <= rhs));
  }

  SECTION("Between returns true for floats")
  {
    auto lhs = GenerateInterestingFloats();
    auto rhs = GenerateInterestingFloats();
    auto mid = GenerateInterestingFloats();

    REQUIRE(checker(Between(lhs, rhs), mid) == (lhs <= mid && mid <= rhs));
  }
}

TEST_CASE("BetweenGetter", "[between]")
{
  auto checker = DefaultConditionChecker();

  SECTION("Between with a getter returns true for integers")
  {
    auto lhs = GENERATE(
      Data(std::numeric_limits<int>::min()), Data(-1), Data(0), Data(1), Data(std::numeric_limits<int>::max()));
    auto rhs = GENERATE(
      Data(std::numeric_limits<int>::min()), Data(-1), Data(0), Data(1), Data(std::numeric_limits<int>::max()));
    auto middle = GENERATE(
      Data(std::numeric_limits<int>::min()), Data(-1), Data(0), Data(1), Data(std::numeric_limits<int>::max()));

    REQUIRE(checker(Between(Val<int>(), lhs, rhs), middle) == (lhs.val <= middle.val && middle.val <= rhs.val));
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

    REQUIRE(checker(Between(Val<double>(), lhs, rhs), middle) == (lhs.val <= middle.val && middle.val <= rhs.val));
  }
}
