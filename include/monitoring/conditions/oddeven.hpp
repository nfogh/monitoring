#pragma once

#include "intern.hpp"

namespace Monitoring {

template<typename GetterT> auto Even(GetterT getter)
{
  return [getter = std::move(getter)](const auto value) { return getter(value) % 2 == 0; };
}

auto Even() { return Even(Intern::Identity<int>()); }

template<typename GetterT> auto Odd(GetterT getter)
{
  return [getter = std::move(getter)](const auto value) { return getter(value) % 2 != 0; };
}

auto Odd() { return Odd(Intern::Identity<int>()); }

}// namespace Monitoring