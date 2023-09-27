#pragma once

#include "FloatMessage.hpp"
#include "IntMessage.hpp"
#include <monitoring/conditions/minmax.hpp>

namespace Monitoring {
template<typename T, typename GetterT, typename CompT> auto ParamComparison(GetterT getter, T otherVal, CompT comp)
{
  auto lamb = [otherVal = std::move(otherVal), getter = std::move(getter), comp = std::move(comp), subscribed = false](
                auto &subscriber, const auto &val) mutable {
    if (!subscribed) {
      subscriber(otherVal);
      subscribed = true;
    }
    return comp(otherVal, getter(val));
  };
  return lamb;
}

template<typename Getter> auto Max(Getter getter, PubSubMonitoring::FloatMessage max)
{
  return ParamComparison(std::move(getter), std::move(max), std::greater_equal<PubSubMonitoring::FloatMessage>());
}

template<> auto Max(PubSubMonitoring::FloatMessage max)
{
  return Max(Intern::Identity<PubSubMonitoring::FloatMessage>(), std::move(max));
}

template<typename Getter> auto Min(Getter getter, PubSubMonitoring::FloatMessage min)
{
  return ParamComparison(std::move(getter), std::move(min), std::less_equal<PubSubMonitoring::FloatMessage>());
}

template<> auto Min(PubSubMonitoring::FloatMessage min)
{
  return Min(Intern::Identity<PubSubMonitoring::FloatMessage>(), std::move(min));
}

}// namespace Monitoring
