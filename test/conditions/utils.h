#pragma once

#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>

template<typename T> struct DataT
{
  explicit DataT(T init) : val(init) {}
  T val;
};

template<typename T> auto Data(T init) { return DataT<T>(init); }

template<typename T> auto Val()
{
  return [](const DataT<T> &data) { return data.val; };
}

inline auto GenerateInterestingFloats(const double min = std::numeric_limits<double>::lowest(),
  const double max = std::numeric_limits<double>::max())
{
  return GENERATE_COPY(min, -std::numeric_limits<double>::min(), 0.0, std::numeric_limits<double>::min(), max);
}

inline auto GenerateInterestingInts(const int min = std::numeric_limits<int>::min(),
  const int max = std::numeric_limits<int>::max())
{
  return GENERATE_COPY(min, -1, 0, 1, max);
}
