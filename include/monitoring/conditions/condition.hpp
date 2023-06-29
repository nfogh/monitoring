#pragma once

#include "conditiontypes.hpp"
#include <functional>
#include <type_traits>
#include <utility>

// A condition is a callable which returns a bool and takes a number of const references

#define LIBMONITORING_DEFINE_LOGIC_OPERATORS                                      \
  template<typename OtherCallableT> auto operator||(OtherCallableT otherCallable) \
  {                                                                               \
    return OrCondition(*this, std::move(otherCallable));                          \
  }                                                                               \
                                                                                  \
  template<typename OtherCallableT> auto operator&&(OtherCallableT otherCallable) \
  {                                                                               \
    return AndCondition(*this, std::move(otherCallable));                         \
  }                                                                               \
                                                                                  \
  auto operator!() { return NotCondition(*this); }

namespace Monitoring {

template<typename T, typename = void> struct has_check_method : std::false_type
{
};

struct DefaultConditionChecker
{
  using is_condition_checker = void;

  template<typename CallableT, typename... ArgsT>
  constexpr bool operator()(CallableT &&callable, const ArgsT &...args) const
  {
    if constexpr (needs_checker_v<CallableT>)
      return callable.check(*this, args...);
    else if constexpr (std::is_invocable_v<CallableT, const ArgsT &...>)
      return std::invoke(callable, args...);
    else
      return callable.check(args...);
  }
};

template<typename LhsT, typename RhsT> struct AndCondition
{
  AndCondition(LhsT lhs, RhsT rhs) : mLhs(std::move(lhs)), mRhs(std::move(rhs)) {}

  using needs_checker = void;

  template<typename CheckerT, typename... ArgsT>
  constexpr bool check(const CheckerT &checker, const ArgsT &...args) const
  {
    return checker(mLhs, args...) && checker(mRhs, args...);
  }

  LIBMONITORING_DEFINE_LOGIC_OPERATORS;

private:
  LhsT mLhs;
  RhsT mRhs;
};

template<typename LhsT, typename RhsT> struct OrCondition
{
  OrCondition(LhsT lhs, RhsT rhs) : mLhs(std::move(lhs)), mRhs(std::move(rhs)) {}

  using needs_checker = void;

  template<typename CheckerT, typename... ArgsT>
  constexpr bool check(const CheckerT &checker, const ArgsT &...args) const
  {
    return checker(mLhs, args...) || checker(mRhs, args...);
  }

  LIBMONITORING_DEFINE_LOGIC_OPERATORS;

private:
  LhsT mLhs;
  RhsT mRhs;
};

template<typename CallableT> struct NotCondition
{
  NotCondition(CallableT callable) : mCallable(std::move(callable)) {}

  using needs_checker = void;

  template<typename CheckerT, typename... ArgsT>
  constexpr bool check(const CheckerT &checker, const ArgsT &...args) const
  {
    return !checker(mCallable, args...);
  }

  LIBMONITORING_DEFINE_LOGIC_OPERATORS;

private:
  CallableT mCallable;
};

// Wrapping your callables in this class makes it possible to do logical operations on them.
// The callables must return a bool and take the same parameters.
// In fact, only the first callable needs to be wrapped in this class, the others can be normal callables.
template<typename CallableT> struct Condition
{
  Condition(CallableT callable) : mCallable(std::move(callable)) {}

  template<typename CheckerT, typename... ArgsT>
  constexpr bool check(const CheckerT &checker, const ArgsT &...args) const
  {
    return checker(mCallable, args...);
  }

  LIBMONITORING_DEFINE_LOGIC_OPERATORS;

private:
  CallableT mCallable;
};
}// namespace Monitoring
