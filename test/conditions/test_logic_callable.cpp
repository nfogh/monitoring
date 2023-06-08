#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include <monitoring/conditions/logic_callable.hpp>

using namespace Monitoring;

TEST_CASE("logic_callable", "[logic_callable]")
{
  SECTION("Something")
  {
    auto logicCallable = LogicCallable([](auto &&.../*args*/) { return true; });
    auto logicCallable2 = LogicCallable([](auto &&.../*args*/) { return false; });
    REQUIRE((logicCallable && logicCallable2)() == false);
    REQUIRE((logicCallable || logicCallable2)() == true);
    REQUIRE((!logicCallable)() == false);
    REQUIRE((!logicCallable2)() == true);
    REQUIRE((logicCallable && []() { return false; })() == false);
  }
}
