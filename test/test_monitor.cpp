#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include <iostream>
#include <monitoring/conditions/argindex.hpp>
#include <monitoring/conditions/equals.hpp>
#include <monitoring/monitor.hpp>

using namespace Monitoring;

template<class, class = void> struct is_mycheck : std::false_type
{
};
template<class T> struct is_mycheck<T, std::void_t<typename std::decay_t<T>::is_mycheck>> : std::true_type
{
};
template<typename T> inline constexpr bool is_mycheck_v = is_mycheck<T>::value;

struct MyChecker
{
  using is_mycheck = void;
  template<typename ValT> bool check(const std::function<void(std::string_view)> &callback, const ValT &val) const
  {
    std::cout << "Called" << std::endl;
    callback("Hej hej from " + std::to_string(val));
    return true;
  }

  LIBMONITORING_DEFINE_LOGIC_OPERATORS;
};

struct MyConditionChecker
{
  using is_condition_checker = void;

  MyConditionChecker(std::function<void(std::string_view)> callback) : mCallback(std::move(callback)){};

  template<typename CallableT, typename... ArgsT>
  constexpr bool operator()(CallableT &&callable, const ArgsT &...args) const
  {
    if constexpr (needs_checker_v<CallableT>)
      return callable.check(*this, args...);
    else if constexpr (is_mycheck_v<CallableT>)
      return callable.check(mCallback, args...);
    else if constexpr (std::is_invocable_v<CallableT, const ArgsT &...>)
      return std::invoke(callable, args...);
    else
      return callable.check(args...);
  }

private:
  std::function<void(std::string_view)> mCallback;
};

TEST_CASE("Monitor", "[monitor]")
{
  SECTION("monitor")
  {
    Monitor(Equals(1), [](const bool success) { REQUIRE(success); })(1);
    Monitor(
      Equals(1), [](const bool success) { REQUIRE(success); }, [](const bool success) { REQUIRE(success); })(1);
    Monitor(Equals(1.0), [](const bool success) { REQUIRE(success); })(1.0);
    Monitor(FirstArg(Equals(1.0)), [](const bool success) { REQUIRE(success); })(1.0, 2.0);
  }

  SECTION("MyMonitor")
  {
    std::cout << "MyMonitor" << std::endl;
    Monitor(Equals(1) && MyChecker(),
      MyConditionChecker([](std::string_view sv) { std::cout << sv << std::endl; }),
      [](const bool success) { REQUIRE(success); })(1);
  }
}
