#pragma once
#include "include/qgl_model_include.h"

namespace qgl
{
   template<typename TickT>
   class time_state
   {
      static_assert(std::is_signed<TickT>::value &&
                    std::is_integral<TickT>::value,
                    "TickT must be a signed integral.");

      public:
      using this_type = time_state<TickT>;

      time_state() noexcept = default;

      time_state(TickT deltaTicks, TickT totalTicks, uint32_t framesPerSecond) noexcept
         : m_delta(deltaTicks), m_total(totalTicks), m_fps(framesPerSecond)
      {
      }

      time_state(const this_type& other) = default;
      time_state(this_type&& other) noexcept = default;

      ~time_state() = default;

      this_type& operator=(this_type other) noexcept
      {
         swap(*this, other);
         return *this;
      }

      TickT delta_t() const noexcept
      {
         return m_delta;
      }

      TickT ticks() const noexcept
      {
         return m_total;
      }

      uint32_t fps() const noexcept
      {
         return m_fps;
      }

      // Comparison operators
      friend bool operator==(const this_type& a, const this_type& b) noexcept
      {
         return a.m_delta == b.m_delta &&
            a.m_total == b.m_total &&
            a.m_fps == b.m_fps;
      }

      friend bool operator<(const this_type& a, const this_type& b) noexcept
      {
         return std::tie(a.m_total, a.m_delta, a.m_fps) <
            std::tie(b.m_total, b.m_delta, b.m_fps);
      }

      friend void swap(this_type& a, this_type& b) noexcept
      {
         using std::swap;
         swap(a.m_delta, b.m_delta);
         swap(a.m_total, b.m_total);
         swap(a.m_fps, b.m_fps);
      }

      // Derived comparison operators
      friend bool operator!=(const this_type& a, const this_type& b) noexcept
      {
         return !(a == b);
      }

      friend bool operator>(const this_type& a, const this_type& b) noexcept
      {
         return b < a;
      }

      friend bool operator<=(const this_type& a, const this_type& b) noexcept
      {
         return !(b < a);
      }

      friend bool operator>=(const this_type& a, const this_type& b) noexcept
      {
         return !(a < b);
      }

      private:
      TickT m_delta = 0;
      TickT m_total = 0;
      uint32_t m_fps = 0;
   };
}