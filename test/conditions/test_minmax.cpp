#include "utils.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include <monitoring/conditions/minmax.hpp>

using namespace Monitoring;

TEST_CASE("Min", "[min]")
{
  auto checker = DefaultConditionChecker();
  SECTION("Min returns true for integers")
  {
    auto lhs = GenerateInterestingInts();
    auto rhs = GenerateInterestingInts();

    REQUIRE(checker(Min(lhs), rhs) == (lhs <= rhs));
  }

  SECTION("Min returns true for floats")
  {
    auto lhs = GenerateInterestingFloats();
    auto rhs = GenerateInterestingFloats();

    REQUIRE(checker(Min(lhs), rhs) == (lhs <= rhs));
  }
}

TEST_CASE("Max", "[max]")
{
  auto checker = DefaultConditionChecker();
  SECTION("Max returns true for integers")
  {
    auto lhs = GenerateInterestingInts();
    auto rhs = GenerateInterestingInts();

    REQUIRE(checker(Max(lhs), rhs) == (lhs >= rhs));
  }

  SECTION("Max returns true for floats")
  {
    auto lhs = GenerateInterestingFloats();
    auto rhs = GenerateInterestingFloats();

    REQUIRE(checker(Max(lhs), rhs) == (lhs >= rhs));
  }
}

TEST_CASE("MaxGetter", "[max]")
{
  auto checker = DefaultConditionChecker();
  SECTION("Max with a getter returns true for integers")
  {
    auto lhs = GENERATE(
      Data(std::numeric_limits<int>::min()), Data(-1), Data(0), Data(1), Data(std::numeric_limits<int>::max()));
    auto rhs = GENERATE(
      Data(std::numeric_limits<int>::min()), Data(-1), Data(0), Data(1), Data(std::numeric_limits<int>::max()));

    REQUIRE(checker(Max(Val<int>(), lhs.val), rhs) == (lhs.val >= rhs.val));
  }

  SECTION("Max returns true for floats")
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

    REQUIRE(checker(Max(Val<double>(), lhs.val), rhs) == (lhs.val >= rhs.val));
  }
}

TEST_CASE("MaxHyst", "[min]")
{
  auto checker = DefaultConditionChecker();
  SECTION("Maxhyst")
  {
    //      4
    //      3
    // max  2 ----------
    //      1
    // hyst 0 ==========
    //

    auto maxHyst = MaxHyst(2, 2);
    REQUIRE(checker(maxHyst, 0) == true);
    REQUIRE(checker(maxHyst, 1) == true);
    REQUIRE(checker(maxHyst, 3) == false);
    REQUIRE(checker(maxHyst, 4) == false);
    REQUIRE(checker(maxHyst, 1) == false);
    REQUIRE(checker(maxHyst, 0) == true);
    REQUIRE(checker(maxHyst, 1) == true);
  }

  SECTION("MaxhystDouble")
  {
    //      4
    //      3
    // max  2 ----------
    //      1
    // hyst 0 ==========
    //

    auto maxHyst = MaxHyst(2.0, 2.0);
    REQUIRE(checker(maxHyst, 0.0) == true);
    REQUIRE(checker(maxHyst, 1.0) == true);
    REQUIRE(checker(maxHyst, 3.0) == false);
    REQUIRE(checker(maxHyst, 4.0) == false);
    REQUIRE(checker(maxHyst, 1.0) == false);
    REQUIRE(checker(maxHyst, 0.0) == true);
    REQUIRE(checker(maxHyst, 1.0) == true);
  }
}

TEST_CASE("MinHyst", "[min]")
{
  auto checker = DefaultConditionChecker();
  SECTION("Minhyst")
  {
    // hyst 4 ==========
    //      3
    // min  2 ----------
    //      1
    //      0
    //

    auto minHyst = MinHyst(2, 2);
    REQUIRE(checker(minHyst, 3) == true);
    REQUIRE(checker(minHyst, 2) == true);
    REQUIRE(checker(minHyst, 1) == false);
    REQUIRE(checker(minHyst, 2) == false);
    REQUIRE(checker(minHyst, 3) == false);
    REQUIRE(checker(minHyst, 4) == true);
    REQUIRE(checker(minHyst, 3) == true);
  }

  SECTION("MinhystDouble")
  {
    // hyst 4 ==========
    //      3
    // min  2 ----------
    //      1
    //      0
    //

    auto minHyst = MinHyst(2.0, 2.0);
    REQUIRE(checker(minHyst, 3.0) == true);
    REQUIRE(checker(minHyst, 2.0) == true);
    REQUIRE(checker(minHyst, 1.0) == false);
    REQUIRE(checker(minHyst, 2.0) == false);
    REQUIRE(checker(minHyst, 3.0) == false);
    REQUIRE(checker(minHyst, 4.0) == true);
    REQUIRE(checker(minHyst, 3.0) == true);
  }
}
