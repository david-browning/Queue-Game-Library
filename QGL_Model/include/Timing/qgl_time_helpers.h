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

   /*
    Converts a number of ticks to milliseconds. Milliseconds should be a 
    floating point type to avoid truncation.
    While this template function will work fine with floats, a double is less 
    susceptible to rounding errors, especially after the elapsed time is 60 
    seconds.
    */
   template<typename MillisecondsT, typename TickT>
   constexpr MillisecondsT ticks_to_milliseconds(TickT t)
   {
      return static_cast<MillisecondsT>(t) /
         static_cast<MillisecondsT>(TICKS_PER_MILLISECOND);
   }

   /*
    Converts milliseconds to ticks.
    */
   template<typename MillisecondsT, typename TickT>
   constexpr TickT milliseconds_to_ticks(MillisecondsT ms)
   {
      return static_cast<TickT>(ms *
                                static_cast<MillisecondsT>(
                                   TICKS_PER_MILLISECOND)
                                );
   }

   /*
    Converts t ticks into seconds. Seconds should be a floating point type to
    avoid truncation.
    */
   template<typename SecondsT, typename TickT>
   constexpr SecondsT ticks_to_seconds(TickT t)
   {
      return static_cast<SecondsT>(t) /
         static_cast<SecondsT>(TICKS_PER_SECOND);
   }

   /*
    Converts s seconds to ticks. Seconds should be a floating point type to 
    avoid truncation.
    While this template function will work fine with floats, a double is less
    susceptible to rounding errors, especially after the elapsed time is 60
    seconds.
    */
   template<typename SecondsT, typename TickT>
   constexpr TickT seconds_to_ticks(SecondsT s)
   {
      return static_cast<TickT>(s *
                                static_cast<SecondsT>(TICKS_PER_SECOND));
   }
}