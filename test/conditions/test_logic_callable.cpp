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
    [[maybe_unused]] auto constLogicCallable1 = LogicCallable([](auto &&.../*args*/) { return true; });
    [[maybe_unused]] auto constLogicCallable2 = LogicCallable([](auto &&.../*args*/) { return false; });
    [[maybe_unused]] auto nonConstLogicCallable1 = LogicCallable([](auto &&.../*args*/) mutable { return true; });
    [[maybe_unused]] auto nonConstLogicCallable2 = LogicCallable([](auto &&.../*args*/) mutable { return false; });

    static_assert(is_const_invocable_v<decltype(constLogicCallable1), int>,
      "A LogicCallable-wrapped callable which is const invocable should also be const invocable");
    static_assert(!is_const_invocable_v<decltype(nonConstLogicCallable1), int>,
      "A LogicCallable-wrapped callable which is not const invocable should also not be const invocable");

    static_assert(is_const_invocable_v<decltype(constLogicCallable1 && constLogicCallable2), int>,
      "AND of const invocable LogicCallables should be const invocable");
    static_assert(is_const_invocable_v<decltype(constLogicCallable1 || constLogicCallable2), int>,
      "OR of const invocable LogicCallables should be const invocable");
    static_assert(is_const_invocable_v<decltype(!constLogicCallable1), int>,
      "NOT of a const invocable LogicCallable should be const invocable");

    static_assert(!is_const_invocable_v<decltype(constLogicCallable1 && nonConstLogicCallable2), int>,
      "AND of const invocable and non const-invocable LogicCallables should not be const invocable");
    static_assert(!is_const_invocable_v<decltype(constLogicCallable1 || nonConstLogicCallable2), int>,
      "OR of const invocable and non const-invocable LogicCallables should not be const invocable");
    static_assert(!is_const_invocable_v<decltype(!nonConstLogicCallable1), int>,
      "NOT of a non const-invocable LogicCallable should also be non const-invocable");
  }
}
