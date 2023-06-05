#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include <monitoring/conditions.hpp>

using namespace Monitoring;

auto GenerateInterestingFloats()
{
  return GENERATE(std::numeric_limits<double>::lowest(),
    -std::numeric_limits<double>::min(),
    0.0,
    std::numeric_limits<double>::min(),
    std::numeric_limits<double>::max());
}

auto GenerateInterestingInts()
{
  return GENERATE(std::numeric_limits<int>::min(), -1, 0, 1, std::numeric_limits<int>::max());
}

TEST_CASE("Min", "[min]")
{
  SECTION("Min returns true for integers")
  {
    auto i = GenerateInterestingInts();
    auto j = GenerateInterestingInts();

    REQUIRE(Min(i)(j) == i <= j);
  }

  SECTION("Min returns true for floats")
  {
    auto i = GenerateInterestingFloats();
    auto j = GenerateInterestingFloats();

    REQUIRE(Min(i)(j) == i <= j);
  }
}

TEST_CASE("Max", "[max]")
{
  SECTION("Max returns true for integers")
  {
    auto i = GenerateInterestingInts();
    auto j = GenerateInterestingInts();

    CAPTURE(i, j);
    REQUIRE(Max(i)(j) == i >= j);
  }

  SECTION("Max returns true for floats")
  {
    auto i = GenerateInterestingFloats();
    auto j = GenerateInterestingFloats();

    CAPTURE(i, j);
    REQUIRE(Max(i)(j) == i >= j);
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

TEST_CASE("MaxGetter", "[max]")
{
  SECTION("Max with a getter returns true for integers")
  {
    auto i = GENERATE(
      Data(std::numeric_limits<int>::min()), Data(-1), Data(0), Data(1), Data(std::numeric_limits<int>::max()));
    auto j = GENERATE(
      Data(std::numeric_limits<int>::min()), Data(-1), Data(0), Data(1), Data(std::numeric_limits<int>::max()));

    REQUIRE(Max(i, DataGetter<int>())(j) == i.val >= j.val);
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

    REQUIRE(Max(i, DataGetter<double>())(j) == i.val >= j.val);
  }
}

TEST_CASE("MaxHyst", "[min]")
{
  SECTION("Maxhyst")
  {
    //      4
    //      3
    // max  2 ----------
    //      1
    // hyst 0 ==========
    //

    auto maxHyst = MaxHyst(2, 2);
    REQUIRE(maxHyst(0) == true);
    REQUIRE(maxHyst(1) == true);
    REQUIRE(maxHyst(3) == false);
    REQUIRE(maxHyst(4) == false);
    REQUIRE(maxHyst(1) == false);
    REQUIRE(maxHyst(0) == true);
    REQUIRE(maxHyst(1) == true);
  }
}

TEST_CASE("MinHyst", "[min]")
{
  SECTION("Minhyst")
  {
    // hyst 4 ==========
    //      3
    // min  2 ----------
    //      1
    //      0
    //

    auto minHyst = MinHyst(2, 2);
    REQUIRE(minHyst(3) == true);
    REQUIRE(minHyst(2) == true);
    REQUIRE(minHyst(1) == false);
    REQUIRE(minHyst(2) == false);
    REQUIRE(minHyst(3) == false);
    REQUIRE(minHyst(4) == true);
    REQUIRE(minHyst(3) == true);
  }
}
