#pragma once

namespace Monitoring
{
    template<typename PropertyT, typename InnerConditionT>
    auto Property(PropertyT property, InnerConditionT innerCond)
    {
        return [property, innerCond = std::move(innerCond)](const auto& arg){ return innerCond((arg.*property)()); };
    };
}
