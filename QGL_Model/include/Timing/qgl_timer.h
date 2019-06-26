#pragma once
#include "include/qgl_model_include.h"
#include "qgl_time_helpers.h"
#include "qgl_time_state.h"

namespace qgl
{
   template<typename TickT>
   class timer
   {
      public:
      using TimeT = LARGE_INTEGER;

      /*
       Standard target framerates for the timer.
       Count the number of ticks in a given fraction of a second.
       */
      static constexpr TickT TICK_24_HZ = TICKS_PER_SECOND / 24;
      static constexpr TickT TICK_30_HZ = TICKS_PER_SECOND / 30;
      static constexpr TickT TICK_60_HZ = TICKS_PER_SECOND / 60;
      static constexpr TickT TICK_120_HZ = TICKS_PER_SECOND / 120;
      static constexpr TickT TICK_144_HZ = TICKS_PER_SECOND / 144;

      /*
       Constructs a timer and queries the timer's frequency.
       targetHz: The maximum amount of time between time updates. If
        the time between any two calls to tick() is greater than this, the
        elapsed time is clamped to the target. This is to keep the timer
        running at a consistent rate.
       targetTolerance: If the elapsed ticks is within the tolerance of the 
        target, then the elapsed ticks are clamped.
       timerOffset: Time, in ticks, that the timer starts at. Useful for
        debugging rounding errors.
       */
      timer(TickT targetHz = TICK_60_HZ,
            TickT targetTolerance = TICKS_PER_SECOND / 4000,
            TickT timerOffset = 0) :
         m_frameCount(0),
         m_fpsTime(0),
         m_maxDelta(targetHz),
         m_targetTicks(targetHz),
         m_elapsedTicks(0),
         m_totalTicks(0),
         m_leftoverTicks(0),
         m_framesPerSecond(0),
         m_framesThisSecond(0),
         m_targetTolerance(targetTolerance)
      {
         TimeT freq;
         QueryPerformanceFrequency(&freq);
         m_frequency = freq.QuadPart;

         m_lastTime.QuadPart = timerOffset;
      }

      /*
       Copy constructor.
       */
      timer(const timer& r) = default;

      /*
       Move constructor.
       */
      timer(timer&& r) = default;

      /*
       Destructor.
       */
      virtual ~timer() noexcept = default;

      /*
       Returns the target number of ticks per second. If the timer ticks
       close to this value, the elapsed time is clamped to the target ticks
       per second. This is to keep the elapsed time consistent if the engine
       is running quickly.
       Set the tolerance using target_tolerance().
       */
      TickT target_tps() const noexcept
      {
         return m_targetTicks;
      }

      /*
       The tolerance of how close elapsed ticks can be to the target elapsed
       ticks.
       If the elapsed ticks is within the tolerance of the target, then the
       elapsed ticks are clamped.
       */
      TickT target_tolerance() const noexcept
      {
         return m_targetTolerance;
      }

      /*
       Gets the maximum number of ticks that can pass between two calls
       to tick().
       This is useful to keep the timer from suddenly jumping after the
       engine was paused.
       */
      TickT max_ticks() const noexcept
      {
         return m_maxDelta;
      }

      /*
       Returns the number of ticks between the last two calls to tick().
       */
      TickT deltaT() const noexcept
      {
         return m_elapsedTicks;
      }

      /*
       Returns the number of frames per second.
       */
      UINT fps() const noexcept
      {
         return m_framesPerSecond;
      }

      /*
       Returns the number of ticks between when tick() was first called to
       when it was last called.
       */
      TickT ticks() const noexcept
      {
         return m_totalTicks;
      }

      /*
       Returns the state of the timer since it was last updated.
       The state is updated by calling tick().
       */
      time_state<TickT> state() const
      {
         return time_state<TickT>(m_elapsedTicks,
                                  m_totalTicks,
                                  m_framesPerSecond);
      }

      void tick()
      {
         //Get the current time.
         TimeT currentTime;
         QueryPerformanceCounter(&currentTime);

         //Update the delta time and last time.
         TickT deltaTime = currentTime.QuadPart - m_lastTime.QuadPart;
         m_lastTime = currentTime;
         m_fpsTime += deltaTime;

         //Clamp the elapsed ticks.
         //Useful after the program is paused.
         deltaTime = std::clamp<TickT>(deltaTime, 0, m_maxDelta);

         //Convert QPC units into a canonical tick format. This cannot 
         //overflow due to the previous clamp.
         // To guard against loss-of-precision, we convert
         // to microseconds *before* dividing by ticks-per-second.
         deltaTime *= TICKS_PER_SECOND;
         deltaTime /= m_frequency;

         auto lastFrameCount = m_frameCount;

         //If the timer is running within the tolerance to the target 
         //number of ticks per  millisecond, then clamp it to match the 
         //target t/ms. This prevents tiny errors from accumulating over 
         //time.
         //If the elapsed ticks is withing the tolerance of the target, 
         //then the elapsed ticks are clamped.
         if (std::abs(static_cast<long long>(deltaTime - m_targetTicks)) <
             m_targetTolerance)
         {
            deltaTime = m_targetTicks;
         }

         m_leftoverTicks += deltaTime;
         //If there were ticks left over, catch up.
         while (m_leftoverTicks >= m_targetTicks)
         {
            m_elapsedTicks = m_targetTicks;
            m_totalTicks += m_targetTicks;
            m_leftoverTicks -= m_targetTicks;
            m_frameCount++;
         }

         //Track the framerate.
         if (m_frameCount != lastFrameCount)
         {
            m_framesThisSecond++;
         }

         if (m_fpsTime >= m_frequency)
         {
            m_framesPerSecond = m_framesThisSecond;
            m_framesThisSecond = 0;
            m_fpsTime %= m_frequency;
         }
      }

      /*
       Swaps the contents of first and second.
       */
      friend void swap(timer& first,
                       timer& second) noexcept
      {
         using std::swap;
         swap(first.m_lastTime, second.m_lastTime);
         swap(first.m_frequency, second.m_frequency);
         swap(first.m_maxDelta, second.m_maxDelta);
         swap(first.m_targetTicks, second.m_targetTicks);
         swap(first.m_targetTolerance, second.m_targetTolerance);
         swap(first.m_elapsedTicks, second.m_elapsedTicks);
         swap(first.m_totalTicks, second.m_totalTicks);
         swap(first.m_leftoverTicks, second.m_leftoverTicks);
         swap(first.m_fpsTime, second.m_fpsTime);
         swap(first.m_frameCount, second.m_frameCount);
         swap(first.m_framesPerSecond, second.m_framesPerSecond);
         swap(first.m_framesThisSecond, second.m_framesThisSecond);
      }

      /*
       Assignment operator.
       */
      timer& operator=(timer r) noexcept
      {
         swap(*this, r);
         return r;
      }

      private:
      /*
       The elapsed ticks when tick() was last called.
       */
      TimeT m_lastTime;


      /*
       The frequency the timer ticks, in counts per second.
       This is set by the constructor and never changed.
       */
      TickT m_frequency;

      TickT m_fpsTime;
      UINT m_frameCount;
      UINT m_framesPerSecond;
      UINT m_framesThisSecond;

      /*
       Clamp elapsed ticks so it's no bigger than this.
       */
      TickT m_maxDelta;


      /*
       The target elapsed ticks per second. For 60 Hz, set this to
       TICKS_PER_SECOND / 60
       */
      TickT m_targetTicks;

      /*
       The tolerance of how close elapsed ticks can be to the target elapsed
       ticks.
       If the elapsed ticks is within the tolerance of the target, then the
       elapsed ticks are clamped.
       */
      TickT m_targetTolerance;

      /*
       Elapsed ticks between the last two calls to tick().
       */
      TickT m_elapsedTicks;

      /*
       Total number of ticks the clock has been running for.
       */
      TickT m_totalTicks;


      TickT m_leftoverTicks;
   };
}