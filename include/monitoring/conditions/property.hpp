#pragma once

#include "logic_callable.hpp"
#include <utility>
namespace Monitoring
{
    template<typename PropertyT, typename InnerConditionT>
    auto Property(PropertyT property, InnerConditionT innerCond)
    {
        auto lamb = [property, innerCond = std::move(innerCond)](const auto& arg){ return innerCond((arg.*property)()); };
        return LogicCallable(std::move(lamb));
    };
}
