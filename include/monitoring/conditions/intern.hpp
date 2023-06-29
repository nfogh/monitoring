#pragma once

namespace Monitoring::Intern {
struct Identity
{
  template<typename T> constexpr auto operator()(T &&val) const noexcept { return std::forward<T>(val); }
};
}// namespace Monitoring::Intern
