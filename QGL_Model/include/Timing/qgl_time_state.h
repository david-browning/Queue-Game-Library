#pragma once
#include "include/qgl_model_include.h"

namespace qgl
{
   template<typename TickT>
   class time_state
   {
      public:
      time_state(const TickT& deltaT,
                 const TickT& totalT,
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
      virtual ~time_state() noexcept = default;

      inline const TickT& delta_t() const
      {
         return m_deltaTicks;
      }

      inline const TickT& ticks() const
      {
         return m_totalTicks;
      }

      inline UINT fps() const
      {
         return m_fps;
      }

      friend void swap(time_state& first,
                       time_state& second) noexcept
      {
         using std::swap;
         swap(first.m_deltaTicks, second.m_deltaTicks);
         swap(first.m_totalTicks, second.m_totalTicks);
      }

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