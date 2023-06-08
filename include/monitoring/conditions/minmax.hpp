#pragma once

#include "intern.hpp"
#include <iostream>


namespace Monitoring {
template<typename T, typename Getter> auto Max(T max, Getter getter)
{
  return [max = std::move(max), getter = std::move(getter)](const T &val) { return getter(val) <= getter(max); };
}

template<typename T> auto Max(T max) { return Max(std::move(max), Intern::Identity<T>()); }

template<typename T, typename Getter> auto Min(T min, Getter getter)
{
  return [min = std::move(min), getter = std::move(getter)](const T &val) { return getter(val) >= getter(min); };
}

template<typename T> auto Min(T min) { return Min(std::move(min), Intern::Identity<T>()); }

template<typename T, typename Getter> auto MinHyst(T min, T hyst, Getter getter)
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
  return MinHyst(std::move(min), std::move(hyst), Intern::Identity<T>());
}

template<typename T, typename Getter> auto MaxHyst(T max, T hyst, Getter getter)
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
  return MaxHyst(std::move(max), std::move(hyst), Intern::Identity<T>());
}

}// namespace Monitoring
