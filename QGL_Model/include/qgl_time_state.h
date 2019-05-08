#pragma once
#include "include/qgl_model_include.h"

namespace qgl
{
    template<
        typename SecondsT,
        typename TickT,
        typename = std::enable_if<std::is_floating_point<SecondsT>::value>,
        typename = std::enable_if<std::is_integral<TickT>::value &&
            std::is_unsigned<TickT>::value>>
        class time_state
    {
        public:
        time_state(const TickT& deltaT,
                   const TickT& totalT,
                   const SecondsT& deltaS,
                   const SecondsT& totalS,
                   UINT framesps) :
            m_deltaTicks(deltaT),
            m_totalTicks(totalT),
            m_deltaS(deltaS),
            m_totalS(totalS),
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

        inline const SecondsT& delta_s() const
        {
            return m_deltaS;
        }

        inline const SecondsT& seconds() const
        {
            return m_totalS;
        }

        inline const SecondsT& delta_ms() const
        {
            return m_deltaS * static_cast<SecondsT>(1000);
        }

        inline const SecondsT& milliseconds() const
        {
            return m_totalS * static_cast<SecondsT>(1000);
        }

        inline UINT fps() const
        {
            return m_fps;
        }

        friend void swap(time_state& first, time_state& second) noexcept
        {
            using std::swap;
            swap(first.m_deltaTicks, second.deltaTicks);
            swap(first.m_totalTicks, second.m_totalTicks);
            swap(first.m_deltaS, second.m_deltaS);
            swap(first.m_totalS, second.m_totalS);
        }

        time_state& operator=(time_state r) noexcept
        {
            swap(*this, r);
            return *this;
        }

        private:
        TickT m_deltaTicks;
        TickT m_totalTicks;
        SecondsT m_deltaS;
        SecondsT m_totalS;
        UINT m_fps;
    };
}