#pragma once
#include <iostream>

namespace Monitoring {
auto PrintResult(std::ostream &os = std::cout)
{
  return [&os](const bool value) { os << value << "\n"; };
}
}// namespace Monitoring