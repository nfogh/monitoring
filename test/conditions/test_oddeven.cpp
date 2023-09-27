#include "utils.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include <monitoring/conditions/oddeven.hpp>

using namespace Monitoring;

namespace {
constexpr int _ = 0;
}

TEST_CASE("Odd", "[odd]")
{
  SECTION("Odd for integers")
  {
    auto val = GENERATE(-2, -1, 0, 1, 2);
    REQUIRE(Odd()(_, val) == (val % 2 != 0));
  }
}

TEST_CASE("Even", "[even]")
{
  SECTION("Even for integers")
  {
    auto val = GENERATE(-2, -1, 0, 1, 2);
    REQUIRE(Even()(_, val) == (val % 2 == 0));
  }
}

TEST_CASE("OddGetter", "[odd]")
{
  SECTION("Odd with a getter for integers")
  {
    auto val = GENERATE(Data(-2), Data(-1), Data(0), Data(1), Data(2));

    REQUIRE(Even(Val<int>())(_, val) == (val.val % 2 == 0));
  }
}

TEST_CASE("EventGetter", "[even]")
{
  SECTION("Even with a getter for integers")
  {
    auto val = GENERATE(Data(-2), Data(-1), Data(0), Data(1), Data(2));

    REQUIRE(Even(Val<int>())(_, val) == (val.val % 2 == 0));
  }
}