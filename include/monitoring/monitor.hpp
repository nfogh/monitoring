#pragma once
#include <memory>
#include <tuple>
#include <utility>

namespace Monitoring {
namespace Intern {
  template<typename ConditionT, typename HandlersT, typename CheckerT> class Monitor
  {
  public:
    Monitor(ConditionT condition, HandlersT handlers, CheckerT checker)
      : mCondition(std::move(condition)), mChecker(std::move(checker)), mHandlers(std::move(handlers))
    {}

    // Check the given types for the conditions, and call the
    // handlers with the
    template<typename... Args> [[nodiscard]] auto operator()(const Args &...args)
    {
      const bool allOk = mChecker(mCondition, args...);

      if constexpr (std::tuple_size_v<HandlersT> != 0) {
        std::apply(
          [allOk](auto &&...args) { (..., args(allOk)); }, mHandlers);// Call all handlers with allOk as a parameter
      } else {
        return allOk;
      }
    }

  private:
    ConditionT mCondition;
    CheckerT mChecker;
    HandlersT mHandlers;
  };
}// namespace Intern

template<typename ConditionT,
  typename CheckerT,
  typename... HandlersT,
  typename std::enable_if_t<is_condition_checker_v<CheckerT>, int> = 0>
[[nodiscard]] auto Monitor(ConditionT condition, CheckerT checker, HandlersT... handlers)
{
  return Intern::Monitor(std::move(condition), std::make_tuple(std::move(handlers)...), std::move(checker));
}

template<typename ConditionT,
  typename FirstHandlerT,
  typename... OtherHandlersT,
  typename std::enable_if_t<!is_condition_checker_v<FirstHandlerT>, int> = 0>
[[nodiscard]] auto Monitor(ConditionT condition, FirstHandlerT handler, OtherHandlersT... otherHandlers)
{
  return Intern::Monitor(std::move(condition),
    std::tuple_cat(std::tuple(std::move(handler)), std::make_tuple(std::move(otherHandlers)...)),
    DefaultConditionChecker());
}

}// namespace Monitoring
