#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include <monitoring/conditions/logic_callable.hpp>

using namespace Monitoring;

template<typename CallableT, typename... Args>
inline constexpr bool is_const_invocable_v = std::is_invocable_v<const CallableT, Args...>;

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

  SECTION("ConstCorrectness")
  {
    auto logicCallable1 = LogicCallable([](auto &&.../*args*/) { return true; });
    auto logicCallable2 = LogicCallable([](auto &&.../*args*/) { return false; });
    auto logicCallableAnd = logicCallable1 && logicCallable2;
    auto logicCallableOr = logicCallable1 || logicCallable2;
    auto logicCallableNot = !logicCallable1;

    static_assert(is_const_invocable_v<decltype(logicCallable1), int>, "logicCallable should be const invocable");
    static_assert(is_const_invocable_v<decltype(logicCallable2), int>, "logicCallable should be const invocable");

    static_assert(is_const_invocable_v<decltype(logicCallableAnd), int>, "logicCallable should be const invocable");
    static_assert(is_const_invocable_v<decltype(logicCallableOr), int>, "logicCallable should be const invocable");
    static_assert(is_const_invocable_v<decltype(logicCallableNot), int>, "logicCallable should be const invocable");
  }
}
