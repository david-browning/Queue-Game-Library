#pragma once
#include "include/qgl_model_include.h"

namespace qgl
{
   /*
    How many ticks in a millisecond.
    */
   static constexpr uint64_t TICKS_PER_MILLISECOND = 10'000;

   /*
    How many ticks in a second.
    */
   static constexpr uint64_t TICKS_PER_SECOND = TICKS_PER_MILLISECOND * 1000;

   template<typename MillisecondsT, typename TickT>
   inline static MillisecondsT ticks_to_milliseconds(TickT t)
   {
      return static_cast<MillisecondsT>(t) /
         static_cast<MillisecondsT>(TICKS_PER_MILLISECOND);
   }

   template<typename MillisecondsT, typename TickT>
   inline static TickT milliseconds_to_ticks(MillisecondsT ms)
   {
      return static_cast<TickT>(ms *
                                static_cast<MillisecondsT>(
                                   TICKS_PER_MILLISECOND)
                                );
   }

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