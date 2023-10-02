#pragma once

#include "logic_callable.hpp"

namespace Monitoring {
template<typename ClassT, typename FieldT, typename InnerConditionT>
auto Field(FieldT ClassT::*field, InnerConditionT innerCond)
{
  auto lamb = [field, innerCond = std::move(innerCond)](
                const auto &opt, const auto &arg) { return innerCond(opt, arg.*field); };
  return LogicCallable(std::move(lamb));
};
}// namespace Monitoring
