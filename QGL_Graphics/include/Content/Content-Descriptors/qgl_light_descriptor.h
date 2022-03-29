#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics::descriptors
{
#pragma pack(push, 1)
   struct light_descriptor final
   {
      constexpr light_descriptor()
      {

      }

      light_descriptor(const light_descriptor&) = default;

      light_descriptor(light_descriptor&&) = default;

      ~light_descriptor() noexcept = default;

      friend void swap(light_descriptor& l, light_descriptor& r) noexcept
      {
         using std::swap;
         swap(l.color, r.color);
         swap(l.look, r.look);
         swap(l.position, r.position);
         swap(l.intensity, r.intensity);
      }

      light_descriptor& operator=(light_descriptor r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      /*
       Array of 4 rational numbers. Each number represents the R, G, B, and A
       color values. Values must be between 0 and 1.0.
       */
      fixed_buffer<math::rational<uint32_t>, 4> color;

      /*
       Array of 4 rational numbers. Each number represents the X, Y, and Z
       components. The last value is unused.
       */
      fixed_buffer<math::rational<int32_t>, 4> look;

      /*
       Array of 4 rational numbers. Each number represents the X, Y, and Z
       components. The last value is unused.
       */
      fixed_buffer<math::rational<int32_t>, 4> position;

      /*
       Light intensity. Value must be between 0 and 1.0;
       */
      math::rational<uint32_t> intensity{ 1,1 };
   };
#pragma pack(pop)
}