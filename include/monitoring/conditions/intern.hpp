#pragma once

namespace Monitoring::Intern {
template<typename T> auto Identity()
{
  return [](const T &val) { return val; };
}
}// namespace Monitoring::Intern
