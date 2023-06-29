#pragma once

#include "condition.hpp"
#include "intern.hpp"
#include <algorithm>
#include <functional>
#include <utility>

namespace Monitoring {

template<typename GetterT, typename ValueT, typename ComparerT> struct Difference
{
  Difference(GetterT getter, ValueT comp, ComparerT comparer)
    : mGetter(std::move(getter)), mComp(std::move(comp)), mComparer(std::move(comparer))
  {}

  template<typename ValT> constexpr bool check(const ValT &val1, const ValT &val2) const
  {
    return mComparer(std::abs(mGetter(val1) - mGetter(val2)), mComp);
  }

  LIBMONITORING_DEFINE_LOGIC_OPERATORS;

private:
  GetterT mGetter;
  ValueT mComp;
  ComparerT mComparer;
};

template<typename ValueT, typename GetterT> auto MaxDifference(GetterT getter, ValueT max)
{
  return Difference(std::move(getter), std::move(max), std::less_equal<ValueT>());
}
template<typename ValueT, typename GetterT> auto MinDifference(GetterT getter, ValueT min)
{
  return Difference(std::move(getter), std::move(min), std::greater_equal<ValueT>());
}

template<typename ValueT> auto MaxDifference(ValueT max) { return MaxDifference(Intern::Identity(), std::move(max)); }
template<typename ValueT> auto MinDifference(ValueT min) { return MinDifference(Intern::Identity(), std::move(min)); }

}// namespace Monitoring
