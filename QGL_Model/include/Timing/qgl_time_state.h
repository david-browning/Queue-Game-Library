#pragma once
#include "include/qgl_model_include.h"

namespace qgl
{
   template<typename TickT>
   struct time_state
   {
      public:
      time_state(TickT deltaT,
                 TickT totalT,
                 UINT framesps) :
         m_deltaTicks(deltaT),
         m_totalTicks(totalT),
         m_fps(framesps)
      {

      }

      /*
       Copy Constructor
       */
      time_state(const time_state& c) = default;

      /*
       Move Constructor
       */
      time_state(time_state&& r) = default;

      /*
       Destructor
       */
      ~time_state() noexcept = default;

      /*
       Returns the number of ticks between the last two times the timer was
       updated.
       */
      TickT delta_t() const noexcept
      {
         return m_deltaTicks;
      }

      /*
       Returns the total number of ticks that the timer has been running.
       */
      TickT ticks() const noexcept
      {
         return m_totalTicks;
      }

      /*
       Returns the frames per second.
       */
      UINT fps() const noexcept
      {
         return m_fps;
      }

      /*
       Swaps the contents of first and last.
       */
      friend void swap(time_state& first,
                       time_state& second) noexcept
      {
         using std::swap;
         swap(first.m_deltaTicks, second.m_deltaTicks);
         swap(first.m_totalTicks, second.m_totalTicks);
      }

      /*
       Assignment operator.
       */
      time_state& operator=(time_state r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      private:
      TickT m_deltaTicks;
      TickT m_totalTicks;
      UINT m_fps;
   };
}