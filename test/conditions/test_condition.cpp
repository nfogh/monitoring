#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include <monitoring/conditions/condition.hpp>

using namespace Monitoring;

template<typename CallableT, typename... Args>
inline constexpr bool is_const_invocable_v = std::is_invocable_v<const CallableT, Args...>;

TEST_CASE("logic_callable", "[logic_callable]")
{
  SECTION("Something")
  {
    auto trueCondition = Condition([](const auto &.../*args*/) { return true; });
    auto falseCondition = Condition([](const auto &.../*args*/) { return false; });
    REQUIRE(DefaultConditionChecker::check(trueCondition && falseCondition) == false);
    REQUIRE(DefaultConditionChecker::check(trueCondition || falseCondition) == true);
    REQUIRE(DefaultConditionChecker::check(!trueCondition) == false);
    REQUIRE(DefaultConditionChecker::check(!falseCondition) == true);
    REQUIRE(DefaultConditionChecker::check(trueCondition && []() { return false; }) == false);
  }
}
