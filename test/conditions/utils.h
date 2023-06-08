#pragma once

#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>

inline auto GenerateInterestingFloats()
{
  return GENERATE(std::numeric_limits<double>::lowest(),
    -std::numeric_limits<double>::min(),
    0.0,
    std::numeric_limits<double>::min(),
    std::numeric_limits<double>::max());
}

inline auto GenerateInterestingInts()
{
  return GENERATE(std::numeric_limits<int>::min(), -1, 0, 1, std::numeric_limits<int>::max());
}
