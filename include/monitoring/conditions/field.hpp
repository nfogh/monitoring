#pragma once

#include "condition.hpp"

namespace Monitoring {
template<typename ClassT, typename FieldT, typename InnerConditionT> struct Field
{
  Field(FieldT ClassT::*field, InnerConditionT innerCond) : mField(field), mInnerCond(std::move(innerCond)) {}

  using needs_checker = void;

  template<typename CheckerT, typename ValT> constexpr bool check(const CheckerT &checker, const ValT &val) const
  {
    return checker(mInnerCond, val.*mField);
  }

  LIBMONITORING_DEFINE_LOGIC_OPERATORS;

  FieldT ClassT::*mField;
  InnerConditionT mInnerCond;
};

}// namespace Monitoring
