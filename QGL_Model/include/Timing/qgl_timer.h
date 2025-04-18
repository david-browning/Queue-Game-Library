#pragma once
#include "include/qgl_model_include.h"
#include "qgl_time_helpers.h"
#include "qgl_time_state.h"

namespace qgl
{
   template<typename TickT>
   class timer
   {
      static_assert(std::is_signed<TickT>::value&&
                    std::is_integral<TickT>::value,
                    "TickT must be a signed integral.");

      public:
      // High-resolution frequency
      static constexpr TickT TICKS_PER_SECOND = 10'000'000; // 100 ns ticks

      // Standard target frame rates
      static constexpr TickT TICK_24_HZ = TICKS_PER_SECOND / 24;
      static constexpr TickT TICK_30_HZ = TICKS_PER_SECOND / 30;
      static constexpr TickT TICK_60_HZ = TICKS_PER_SECOND / 60;
      static constexpr TickT TICK_120_HZ = TICKS_PER_SECOND / 120;
      static constexpr TickT TICK_144_HZ = TICKS_PER_SECOND / 144;

      explicit timer(
          TickT targetHz = TICK_60_HZ,
          TickT targetTolerance = TICKS_PER_SECOND / 4000,
          TickT timerOffset = 0)
         : m_targetTick(TICKS_PER_SECOND / targetHz),
         m_tolerance(targetTolerance),
         m_startTick(query_ticks() - timerOffset),
         m_accumulated(0),
         m_frameCount(0),
         m_lastFpsTime(m_startTick),
         m_currentFps(0),
         m_lastTick(m_startTick)
      {}

      void tick(std::function<void()> fixedUpdate)
      {
         TickT now = query_ticks();
         TickT delta = now - m_lastTick;
         m_lastTick = now;
         m_accumulated += delta;

         while (m_accumulated >= m_targetTick - m_tolerance)
         {
            fixedUpdate();
            m_accumulated -= m_targetTick;
            m_frameCount++;
         }

         if (now - m_lastFpsTime >= TICKS_PER_SECOND)
         {
            m_currentFps = static_cast<uint32_t>(m_frameCount);
            m_frameCount = 0;
            m_lastFpsTime = now;
         }
      }

      time_state<TickT> state() const noexcept
      {
         return time_state<TickT>{m_targetTick, total_ticks(), fps()};
      }

      uint32_t fps() const noexcept
      {
         return m_currentFps;
      }

      TickT total_ticks() const noexcept
      {
         return m_lastTick - m_startTick;
      }

      TickT accumulated_ticks() const noexcept
      {
         return m_accumulated;
      }

      TickT last_tick() const noexcept
      {
         return m_lastTick;
      }

      friend void swap(timer<TickT>& a, timer<TickT>& b) noexcept
      {
         using std::swap;
         swap(a.m_targetTick, b.m_targetTick);
         swap(a.m_tolerance, b.m_tolerance);
         swap(a.m_startTick, b.m_startTick);
         swap(a.m_accumulated, b.m_accumulated);
         swap(a.m_lastTick, b.m_lastTick);
         swap(a.m_frameCount, b.m_frameCount);
         swap(a.m_lastFpsTime, b.m_lastFpsTime);
         swap(a.m_currentFps, b.m_currentFps);
      }

      timer<TickT>& operator=(timer<TickT> other) noexcept
      {
         swap(*this, other);
         return *this;
      }

      private:
      TickT m_targetTick;
      TickT m_tolerance;
      TickT m_startTick;
      TickT m_accumulated;
      TickT m_lastTick;

      // FPS tracking
      uint64_t m_frameCount;
      TickT m_lastFpsTime;
      uint32_t m_currentFps;

      TickT query_ticks() const noexcept
      {
         LARGE_INTEGER counter;
         QueryPerformanceCounter(&counter);

         static const TickT ticksPerSecond = []
         {
            LARGE_INTEGER freq;
            QueryPerformanceFrequency(&freq);
            return static_cast<TickT>(freq.QuadPart);
         }();

         // Convert to 100ns resolution
         return static_cast<TickT>((counter.QuadPart * TICKS_PER_SECOND) / ticksPerSecond);
      }
   };
}