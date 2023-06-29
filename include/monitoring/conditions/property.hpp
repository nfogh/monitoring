#pragma once

#include "condition.hpp"
#include <utility>
namespace Monitoring {
template<typename PropertyT, typename InnerConditionT> struct Property
{
  Property(PropertyT property, InnerConditionT innerCond)
    : mProperty(std::move(property)), mInnerCond(std::move(innerCond))
  {}

  using needs_checker = void;

  template<typename CheckerT, typename ValT> bool check(const CheckerT &checker, const ValT &val) const
  {
    return checker(mInnerCond, (val.*mProperty)());
  }

  LIBMONITORING_DEFINE_LOGIC_OPERATORS;

private:
  PropertyT mProperty;
  InnerConditionT mInnerCond;
};
}// namespace Monitoring
