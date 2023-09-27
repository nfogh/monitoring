#include "utils.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <cstdlib>
#include <limits>
#include <monitoring/conditions/difference.hpp>
using namespace Monitoring;

namespace {
constexpr int _ = 0;
}

TEST_CASE("MaxDifference", "[maxdifference]")
{
  SECTION("integers")
  {
    // /2 to avoid integer overflow
    auto val1 = GenerateInterestingInts(std::numeric_limits<int>::min() / 2, std::numeric_limits<int>::max() / 2);
    auto val2 = GenerateInterestingInts(std::numeric_limits<int>::min() / 2, std::numeric_limits<int>::max() / 2);
    auto diff = GenerateInterestingInts(std::numeric_limits<int>::min() / 2, std::numeric_limits<int>::max() / 2);

    CAPTURE(val1, val2, diff);
    REQUIRE(MaxDifference(diff)(_, val1, val2) == (std::abs(val1 - val2) <= diff));
  }

  SECTION("floats")
  {
    auto val1 = GenerateInterestingFloats();
    auto val2 = GenerateInterestingFloats();
    auto diff = GenerateInterestingFloats();

    CAPTURE(val1, val2, diff);
    REQUIRE(MaxDifference(diff)(_, val1, val2) == (std::abs(val1 - val2) <= diff));
  }

  SECTION("integergetter")
  {
    // /2 to avoid integer overflow
    auto val1 = GENERATE(
      Data(std::numeric_limits<int>::min() / 2), Data(-1), Data(0), Data(1), Data(std::numeric_limits<int>::max() / 2));
    auto val2 = GENERATE(
      Data(std::numeric_limits<int>::min() / 2), Data(-1), Data(0), Data(1), Data(std::numeric_limits<int>::max() / 2));
    auto diff = GENERATE(
      Data(std::numeric_limits<int>::min() / 2), Data(-1), Data(0), Data(1), Data(std::numeric_limits<int>::max() / 2));

    REQUIRE(MaxDifference(Val<int>(), diff.val)(_, val1, val2) == (std::abs(val1.val - val2.val) <= diff.val));
  }

  SECTION("floatgetter")
  {
    auto val1 = GENERATE(Data(std::numeric_limits<double>::min()),
      Data(-1.0),
      Data(0.0),
      Data(1.0),
      Data(std::numeric_limits<double>::max()));
    auto val2 = GENERATE(Data(std::numeric_limits<double>::min()),
      Data(-1.0),
      Data(0.0),
      Data(1.0),
      Data(std::numeric_limits<double>::max()));
    auto diff = GENERATE(Data(std::numeric_limits<double>::min()),
      Data(-1.0),
      Data(0.0),
      Data(1.0),
      Data(std::numeric_limits<double>::max()));

    REQUIRE(MaxDifference(Val<double>(), diff.val)(_, val1, val2) == (std::abs(val1.val - val2.val) <= diff.val));
  }
}

TEST_CASE("MinDifference", "[mindifference]")
{
  SECTION("integers")
  {
    // /2 to avoid integer overflow
    auto val1 = GenerateInterestingInts(std::numeric_limits<int>::min() / 2, std::numeric_limits<int>::max() / 2);
    auto val2 = GenerateInterestingInts(std::numeric_limits<int>::min() / 2, std::numeric_limits<int>::max() / 2);
    auto diff = GenerateInterestingInts(std::numeric_limits<int>::min() / 2, std::numeric_limits<int>::max() / 2);

    CAPTURE(val1, val2, diff);
    REQUIRE(MinDifference(diff)(_, val1, val2) == (std::abs(val1 - val2) >= diff));
  }

  SECTION("floats")
  {
    auto val1 = GenerateInterestingFloats();
    auto val2 = GenerateInterestingFloats();
    auto diff = GenerateInterestingFloats();

    CAPTURE(val1, val2, diff);
    REQUIRE(MinDifference(diff)(_, val1, val2) == (std::abs(val1 - val2) >= diff));
  }

  SECTION("integergetter")
  {
    // /2 to avoid integer overflow
    auto val1 = GENERATE(
      Data(std::numeric_limits<int>::min() / 2), Data(-1), Data(0), Data(1), Data(std::numeric_limits<int>::max() / 2));
    auto val2 = GENERATE(
      Data(std::numeric_limits<int>::min() / 2), Data(-1), Data(0), Data(1), Data(std::numeric_limits<int>::max() / 2));
    auto diff = GENERATE(
      Data(std::numeric_limits<int>::min() / 2), Data(-1), Data(0), Data(1), Data(std::numeric_limits<int>::max() / 2));

    REQUIRE(MinDifference(Val<int>(), diff.val)(_, val1, val2) == (std::abs(val1.val - val2.val) >= diff.val));
  }

  SECTION("floatgetter")
  {
    auto val1 = GENERATE(Data(std::numeric_limits<double>::min()),
      Data(-1.0),
      Data(0.0),
      Data(1.0),
      Data(std::numeric_limits<double>::max()));
    auto val2 = GENERATE(Data(std::numeric_limits<double>::min()),
      Data(-1.0),
      Data(0.0),
      Data(1.0),
      Data(std::numeric_limits<double>::max()));
    auto diff = GENERATE(Data(std::numeric_limits<double>::min()),
      Data(-1.0),
      Data(0.0),
      Data(1.0),
      Data(std::numeric_limits<double>::max()));

    REQUIRE(MinDifference(Val<double>(), diff.val)(_, val1, val2) == (std::abs(val1.val - val2.val) >= diff.val));
  }
}
