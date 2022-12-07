#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Descriptors/qgl_vector_descriptor.h"

namespace qgl::descriptors
{
#pragma pack(push, 1)
   struct light_descriptor final
   {
      constexpr light_descriptor()
      {

      }

      constexpr light_descriptor(const light_descriptor&) = default;

      constexpr light_descriptor(light_descriptor&&) = default;

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
      vector_descriptor color;

      /*
       Array of 4 rational numbers. Each number represents the X, Y, and Z
       components. The last value is unused.
       */
      vector_descriptor look;

      /*
       Array of 4 rational numbers. Each number represents the X, Y, and Z
       components. The last value is unused.
       */
      vector_descriptor position;

      /*
       Light intensity. Value must be between 0 and 1.0;
       */
      math::rational<uint32_t> intensity{ 1,1 };
   };
#pragma pack(pop)
}