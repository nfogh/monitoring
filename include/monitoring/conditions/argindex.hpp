#pragma once

#include "condition.hpp"
#include <cstddef>
#include <tuple>

namespace Monitoring {
namespace Intern {
  template<typename InnerConditionT, size_t... ArgIndices> struct Args
  {
    Args(InnerConditionT innerCondition) : mInnerCondition(std::move(innerCondition)) {}

    using needs_checker = void;

    template<typename CheckerT, typename... ArgsT>
    constexpr bool check(const CheckerT &checker, const ArgsT &...args) const
    {
      const auto argsTuple = std::forward_as_tuple(args...);
      return checker(mInnerCondition, std::get<ArgIndices>(argsTuple)...);
    }

    LIBMONITORING_DEFINE_LOGIC_OPERATORS;

  private:
    InnerConditionT mInnerCondition;
  };
}// namespace Intern

template<size_t... ArgIndices, typename InnerConditionT> auto Args(InnerConditionT innerCondition)
{
  return Intern::Args<InnerConditionT, ArgIndices...>(std::move(innerCondition));
}

//@brief Picks out the first argument
template<typename InnerConditionT> auto FirstArg(InnerConditionT innerCondition)
{
  return Args<0>(std::move(innerCondition));
};

//@brief Picks out the second argument
template<typename InnerConditionT> auto SecondArg(InnerConditionT innerCondition)
{
  return Args<1>(std::move(innerCondition));
};

//@brief Picks out the third argument
template<typename InnerConditionT> auto ThirdArg(InnerConditionT innerCondition)
{
  return Args<2>(std::move(innerCondition));
};

//@brief Picks out the fourth argument
template<typename InnerConditionT> auto FourthArg(InnerConditionT innerCondition)
{
  return Args<3>(std::move(innerCondition));
};
}// namespace Monitoring