#pragma once

namespace Monitoring
{
    template<typename ClassT, typename FieldT, typename InnerConditionT>
    auto Field(FieldT ClassT::*field, InnerConditionT innerCond)
    {
        return [field, innerCond = std::move(innerCond)](const auto& arg){ return innerCond(arg.*field); };
    };
}
