#pragma once

namespace Monitoring {
namespace Intern {
  template<typename T> auto Identity()
  {
    return [](const T &val) { return val; };
  }
}// namespace Intern
}// namespace Monitoring