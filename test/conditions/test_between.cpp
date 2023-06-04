#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include <monitoring/conditions/between.hpp>

using namespace Monitoring;

static auto GenerateInterestingFloats()
{
  return GENERATE(std::numeric_limits<double>::lowest(),
    -std::numeric_limits<double>::min(),
    0.0,
    std::numeric_limits<double>::min(),
    std::numeric_limits<double>::max());
}

static auto GenerateInterestingInts()
{
  return GENERATE(std::numeric_limits<int>::min(), -1, 0, 1, std::numeric_limits<int>::max());
}

TEST_CASE("Between", "[between]")
{
  SECTION("Between returns true for integers")
  {
    auto i = GenerateInterestingInts();
    auto j = GenerateInterestingInts();
    auto k = GenerateInterestingInts();

    CAPTURE(i, j, k);
    REQUIRE(Between(i, k)(j) == (i <= j && j <= k));
  }

  SECTION("Between returns true for floats")
  {
    auto i = GenerateInterestingFloats();
    auto j = GenerateInterestingFloats();
    auto k = GenerateInterestingFloats();

    CAPTURE(i, j, k);
    REQUIRE(Between(i, k)(j) == (i <= j && j <= k));
  }
}

template<typename T> struct Data
{
  explicit Data(T val) : val(val) {}
  T val;
};

template<typename T> auto DataGetter()
{
  return [](const Data<T> &data) { return data.val; };
}

TEST_CASE("BetweenGetter", "[between]")
{

  SECTION("Between with a getter returns true for integers")
  {
    auto i = GENERATE(
      Data(std::numeric_limits<int>::min()), Data(-1), Data(0), Data(1), Data(std::numeric_limits<int>::max()));
    auto j = GENERATE(
      Data(std::numeric_limits<int>::min()), Data(-1), Data(0), Data(1), Data(std::numeric_limits<int>::max()));
    auto k = GENERATE(
      Data(std::numeric_limits<int>::min()), Data(-1), Data(0), Data(1), Data(std::numeric_limits<int>::max()));

    REQUIRE(Between(i, k, DataGetter<int>())(j) == (i.val <= j.val && j.val <= k.val));
  }

  SECTION("Max returns true for floats")
  {
    auto i = GENERATE(Data(std::numeric_limits<double>::lowest()),
      Data(-std::numeric_limits<double>::min()),
      Data(0.0),
      Data(std::numeric_limits<double>::min()),
      Data(std::numeric_limits<double>::max()));
    auto j = GENERATE(Data(std::numeric_limits<double>::lowest()),
      Data(-std::numeric_limits<double>::min()),
      Data(0.0),
      Data(std::numeric_limits<double>::min()),
      Data(std::numeric_limits<double>::max()));
    auto k = GENERATE(Data(std::numeric_limits<double>::lowest()),
      Data(-std::numeric_limits<double>::min()),
      Data(0.0),
      Data(std::numeric_limits<double>::min()),
      Data(std::numeric_limits<double>::max()));

    REQUIRE(Between(i, k, DataGetter<double>())(j) == (i.val <= j.val && j.val <= k.val));
  }
}
