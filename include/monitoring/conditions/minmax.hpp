#pragma once

#include "intern.hpp"
#include "logic_callable.hpp"
#include <iostream>


namespace Monitoring {
template<typename T, typename GetterT, typename CompT> auto Comparison(GetterT getter, T otherVal, CompT comp)
{
  auto lamb = [otherVal = std::move(otherVal), getter = std::move(getter), comp = std::move(comp)](
                const T &val) { return comp(getter(otherVal), getter(val)); };
  return LogicCallable(std::move(lamb));
}

template<typename T, typename Getter> auto Max(Getter getter, T max)
{
  return Comparison(std::move(getter), std::move(max), std::greater_equal<std::invoke_result_t<Getter, T>>());
}

template<typename T> auto Max(T max) { return Max(Intern::Identity<T>(), std::move(max)); }

template<typename T, typename Getter> auto Min(Getter getter, T min)
{
  return Comparison(std::move(getter), std::move(min), std::less_equal<std::invoke_result_t<Getter, T>>());
}

template<typename T> auto Min(T min) { return Min(Intern::Identity<T>(), std::move(min)); }

template<typename T, typename Getter> auto MinHyst(Getter getter, T min, T hyst)
{
  return
    [min = std::move(min), hyst = std::move(hyst), getter = std::move(getter), inHyst = false](const T &val) mutable {
      if (inHyst) {
        inHyst = getter(val) < getter(min) + getter(hyst);
      } else {
        inHyst = getter(val) < getter(min);
      }
      return !inHyst;
    };
}

template<typename T> auto MinHyst(T min, T hyst)
{
  return MinHyst(Intern::Identity<T>(), std::move(min), std::move(hyst));
}

template<typename T, typename Getter> auto MaxHyst(Getter getter, T max, T hyst)
{
  return
    [max = std::move(max), hyst = std::move(hyst), getter = std::move(getter), inHyst = false](const T &val) mutable {
      if (inHyst) {
        inHyst = getter(val) > getter(max) - getter(hyst);
      } else {
        inHyst = getter(val) > getter(max);
      }
      return !inHyst;
    };
}

template<typename T> auto MaxHyst(T max, T hyst)
{
  return MaxHyst(Intern::Identity<T>(), std::move(max), std::move(hyst));
}

}// namespace Monitoring
