#pragma once
#include "include/qgl_model_include.h"

namespace qgl
{
    /*
     How many ticks in a second.
     */
    static constexpr uint64_t TICKS_PER_SECOND = 10'000'000;

    /*
     Converts t ticks into seconds.
     */
    template<typename SecondsT, typename TickT>
    inline static SecondsT ticks_to_seconds(TickT t)
    {
        return static_cast<SecondsT>(t) /
            static_cast<SecondsT>(TICKS_PER_SECOND);
    }

    /*
     Converts s seconds to ticks.
     */
    template<typename SecondsT, typename TickT>
    inline static TickT seconds_to_ticks(SecondsT s)
    {
        return static_cast<TickT>(s *
                                  static_cast<SecondsT>(TICKS_PER_SECOND));
    }
}