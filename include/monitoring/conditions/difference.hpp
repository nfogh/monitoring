#pragma once

#include "intern.hpp"
#include "logic_callable.hpp"
#include <algorithm>
#include <utility>
#include <functional>

namespace Monitoring {

template<typename GetterT, typename ValueT, typename ComparerT> auto Difference(GetterT getter, ValueT comp, ComparerT comparer)
{
  auto lamb = [comp = std::move(comp), getter = std::move(getter), comparer = std::move(comparer)](
                const ValueT &val1, const ValueT& val2) { return comparer(std::abs(getter(val1) - getter(val2)), getter(comp)); };
  return LogicCallable<decltype(lamb)>(std::move(lamb));
}

template<typename ValueT, typename GetterT> auto MaxDifference(GetterT getter, ValueT max)
{
  return Difference(std::move(getter), std::move(max), std::less_equal<std::invoke_result_t<GetterT, ValueT>>());
}
template<typename ValueT, typename GetterT> auto MinDifference(GetterT getter, ValueT min)
{
  return Difference(std::move(getter), std::move(min), std::greater_equal<std::invoke_result_t<GetterT, ValueT>>());
}

template<typename ValueT> auto MaxDifference(ValueT max) { return MaxDifference(Intern::Identity<ValueT>(), std::move(max)); }
template<typename ValueT> auto MinDifference(ValueT min) { return MinDifference(Intern::Identity<ValueT>(), std::move(min)); }

}// namespace Monitoring
