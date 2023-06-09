#include "utils.h"
#include <monitoring/conditions/difference.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>

using namespace Monitoring;

TEST_CASE("MaxDifference", "[maxdifference]")
{
  SECTION("integers")
  {
    auto val1 = GenerateInterestingInts();
    auto val2 = GenerateInterestingInts();
    auto diff = GenerateInterestingInts();

    CAPTURE(val1, val2, diff);
    REQUIRE(MaxDifference(diff)(val1, val2) == (std::abs(val1 - val2) <= diff));
  }

  SECTION("floats")
  {
    auto val1 = GenerateInterestingFloats();
    auto val2 = GenerateInterestingFloats();
    auto diff = GenerateInterestingFloats();

    CAPTURE(val1, val2, diff);
    REQUIRE(MaxDifference(diff)(val1, val2) == (std::abs(val1 - val2) <= diff));
  }
}

template<typename T> struct Data
{
  explicit Data(T init) : val(init) {}
  T val;
};

template<typename T> auto Val()
{
  return [](const Data<T> &data) { return data.val; };
}

TEST_CASE("MaxDifferenceGetter", "[maxdifference]")
{
  SECTION("integers")
  {
    auto val1 = GENERATE(
      Data(std::numeric_limits<int>::min()), Data(-1), Data(0), Data(1), Data(std::numeric_limits<int>::max()));
    auto val2 = GENERATE(
      Data(std::numeric_limits<int>::min()), Data(-1), Data(0), Data(1), Data(std::numeric_limits<int>::max()));
    auto diff = GENERATE(
      Data(std::numeric_limits<int>::min()), Data(-1), Data(0), Data(1), Data(std::numeric_limits<int>::max()));

    REQUIRE(MaxDifference(Val<int>(), diff)(val1, val2) == (std::abs(val1.val - val2.val) <= diff.val));
  }

  SECTION("floats")
  {
    auto val1 = GENERATE(
      Data(std::numeric_limits<double>::min()), Data(-1.0), Data(0.0), Data(1.0), Data(std::numeric_limits<double>::max()));
    auto val2 = GENERATE(
      Data(std::numeric_limits<double>::min()), Data(-1.0), Data(0.0), Data(1.0), Data(std::numeric_limits<double>::max()));
    auto diff = GENERATE(
      Data(std::numeric_limits<double>::min()), Data(-1.0), Data(0.0), Data(1.0), Data(std::numeric_limits<double>::max()));

    REQUIRE(MaxDifference(Val<double>(), diff)(val1, val2) == (std::abs(val1.val - val2.val) <= diff.val));
  }

}
