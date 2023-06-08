#include "utils.h"
#include <monitoring/conditions/minmax.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>

using namespace Monitoring;

TEST_CASE("Min", "[min]")
{
  SECTION("Min returns true for integers")
  {
    auto i = GenerateInterestingInts();
    auto j = GenerateInterestingInts();

    REQUIRE(Min(i)(j) == (i <= j));
  }

  SECTION("Min returns true for floats")
  {
    auto i = GenerateInterestingFloats();
    auto j = GenerateInterestingFloats();

    REQUIRE(Min(i)(j) == (i <= j));
  }
}

TEST_CASE("Max", "[max]")
{
  SECTION("Max returns true for integers")
  {
    auto i = GenerateInterestingInts();
    auto j = GenerateInterestingInts();

    CAPTURE(i, j);
    REQUIRE(Max(i)(j) == (i >= j));
  }

  SECTION("Max returns true for floats")
  {
    auto i = GenerateInterestingFloats();
    auto j = GenerateInterestingFloats();

    CAPTURE(i, j);
    REQUIRE(Max(i)(j) == (i >= j));
  }
}

template<typename T> struct Data
{
  explicit Data(T init) : val(init) {}
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

    REQUIRE(Max(i, DataGetter<int>())(j) == (i.val >= j.val));
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

    REQUIRE(Max(i, DataGetter<double>())(j) == (i.val >= j.val));
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

  SECTION("MaxhystDouble")
  {
    //      4
    //      3
    // max  2 ----------
    //      1
    // hyst 0 ==========
    //

    auto maxHyst = MaxHyst(2.0, 2.0);
    REQUIRE(maxHyst(0.0) == true);
    REQUIRE(maxHyst(1.0) == true);
    REQUIRE(maxHyst(3.0) == false);
    REQUIRE(maxHyst(4.0) == false);
    REQUIRE(maxHyst(1.0) == false);
    REQUIRE(maxHyst(0.0) == true);
    REQUIRE(maxHyst(1.0) == true);
  }}

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

  SECTION("MinhystDouble")
  {
    // hyst 4 ==========
    //      3
    // min  2 ----------
    //      1
    //      0
    //

    auto minHyst = MinHyst(2.0, 2.0);
    REQUIRE(minHyst(3.0) == true);
    REQUIRE(minHyst(2.0) == true);
    REQUIRE(minHyst(1.0) == false);
    REQUIRE(minHyst(2.0) == false);
    REQUIRE(minHyst(3.0) == false);
    REQUIRE(minHyst(4.0) == true);
    REQUIRE(minHyst(3.0) == true);
  }
}
