#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include <monitoring/validators.hpp>

using namespace Monitoring;

TEST_CASE("Min", "[min]")
{
  SECTION("Min returns true for integers")
  {
    auto i = GENERATE(std::numeric_limits<int>::min(), -1, 0, 1, std::numeric_limits<int>::max());
    auto j = GENERATE(std::numeric_limits<int>::min(), -1, 0, 1, std::numeric_limits<int>::max());

    REQUIRE(Min(i)(j) == i <= j);
  }

  SECTION("Min returns true for floats")
  {
    auto i = GENERATE(std::numeric_limits<double>::lowest(),
      -std::numeric_limits<double>::min(),
      0.0,
      std::numeric_limits<double>::min(),
      std::numeric_limits<double>::max());
    auto j = GENERATE(std::numeric_limits<double>::lowest(),
      -std::numeric_limits<double>::min(),
      0.0,
      std::numeric_limits<double>::min(),
      std::numeric_limits<double>::max());

    REQUIRE(Min(i)(j) == i <= j);
  }
}

TEST_CASE("Max", "[max]")
{
  SECTION("Max returns true for integers")
  {
    auto i = GENERATE(std::numeric_limits<int>::min(), -1, 0, 1, std::numeric_limits<int>::max());
    auto j = GENERATE(std::numeric_limits<int>::min(), -1, 0, 1, std::numeric_limits<int>::max());

    CAPTURE(i, j);
    REQUIRE(Max(i)(j) == i >= j);
  }

  SECTION("Max returns true for floats")
  {
    auto i = GENERATE(std::numeric_limits<double>::lowest(),
      -std::numeric_limits<double>::min(),
      0.0,
      std::numeric_limits<double>::min(),
      std::numeric_limits<double>::max());
    auto j = GENERATE(std::numeric_limits<double>::lowest(),
      -std::numeric_limits<double>::min(),
      0.0,
      std::numeric_limits<double>::min(),
      std::numeric_limits<double>::max());

    CAPTURE(i, j);
    REQUIRE(Max(i)(j) == i >= j);
  }
}

TEST_CASE("Between", "[between]")
{
  SECTION("Between returns true for integers")
  {
    auto i = GENERATE(std::numeric_limits<int>::min(), -1, 0, 1, std::numeric_limits<int>::max());
    auto j = GENERATE(std::numeric_limits<int>::min(), -1, 0, 1, std::numeric_limits<int>::max());
    auto k = GENERATE(std::numeric_limits<int>::min(), -1, 0, 1, std::numeric_limits<int>::max());

    CAPTURE(i, j, k);
    REQUIRE(Between(i, k)(j) == (i <= j && j <= k));
  }

  SECTION("Between returns true for floats")
  {
    auto i = GENERATE(std::numeric_limits<double>::lowest(),
      -std::numeric_limits<double>::min(),
      0.0,
      std::numeric_limits<double>::min(),
      std::numeric_limits<double>::max());
    auto j = GENERATE(std::numeric_limits<double>::lowest(),
      -std::numeric_limits<double>::min(),
      0.0,
      std::numeric_limits<double>::min(),
      std::numeric_limits<double>::max());
    auto k = GENERATE(std::numeric_limits<double>::lowest(),
      -std::numeric_limits<double>::min(),
      0.0,
      std::numeric_limits<double>::min(),
      std::numeric_limits<double>::max());

    CAPTURE(i, j, k);
    REQUIRE(Between(i, k)(j) == (i <= j && j <= k));
  }
}

template<typename T> struct Data
{
  T i;

  bool operator>=(const Data &other) const noexcept { return i >= other.i; }
};

template<typename T> auto DataGetter()
{
  return [](const Data<T> &data) { return data.i; };
}

TEST_CASE("MaxGetter", "[max]")
{

  SECTION("Max with a getter returns true for integers")
  {
    auto i = GENERATE(map<Data<int>>([](const int &i) { return Data<int>{ i }; },
      take(50, random(std::numeric_limits<int>::min(), std::numeric_limits<int>::max()))));
    auto j = GENERATE(map<Data<int>>([](const int &j) { return Data<int>{ j }; },
      take(50, random(std::numeric_limits<int>::min(), std::numeric_limits<int>::max()))));

    REQUIRE(Max(i, DataGetter<int>())(j) == i >= j);
  }

  SECTION("Max returns true for floats")
  {
    auto i = GENERATE(map<Data<double>>([](const double &i) { return Data<double>{ i }; },
      take(50, random(std::numeric_limits<double>::min(), std::numeric_limits<double>::max()))));
    auto j = GENERATE(map<Data<double>>([](const double &j) { return Data<double>{ j }; },
      take(50, random(std::numeric_limits<double>::min(), std::numeric_limits<double>::max()))));

    REQUIRE(Max(i, DataGetter<double>())(j) == i >= j);
  }
}
