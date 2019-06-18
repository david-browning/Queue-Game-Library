#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::content::buffers
{
   #pragma pack(push, 1)
   struct QGL_GRAPHICS_API LIGHT_BUFFER final
   {
      public:
      /*
       Default Constructor.
       Creates a white light at position 0,0,0 looking at 0,0,0.
       */
      LIGHT_BUFFER();

      /*
       Copy constructor.
       */
      LIGHT_BUFFER(const LIGHT_BUFFER&) = default;

      /*
       Move constructor.
       */
      LIGHT_BUFFER(LIGHT_BUFFER&&) = default;

      /*
       Destructor. Do not mark as virtual.
       */
      ~LIGHT_BUFFER() noexcept = default;

      friend void swap(LIGHT_BUFFER& l,
                       LIGHT_BUFFER& r) noexcept
      {
         using std::swap;
         swap(l.Color, r.Color);
         swap(l.LookAt, r.LookAt);
         swap(l.Position, r.Position);
         swap(l.Intensity, r.Intensity);
         swap(l.Flags1, r.Flags1);
         swap(l.Flags2, r.Flags2);
      }

      LIGHT_BUFFER& operator=(LIGHT_BUFFER r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      /*
       Array of 4 rational numbers. Each number represents the R, G, B, and A 
       color values. Values must be between 0 and 1.0.
       */
      math::rational<int32_t> Color[4];

      /*
       Array of 4 rational numbers. Each number represents the X, Y, and Z 
       components. The last value is unused.
       */
      math::rational<int32_t> LookAt[4];

      /*
       Array of 4 rational numbers. Each number represents the X, Y, and Z 
       components. The last value is unused.
       */
      math::rational<int32_t> Position[4];

      /*
       Light intensity. Value must be between 0 and 1.0;
       */
      math::rational<int32_t> Intensity;
      uint32_t Flags1;
      uint32_t Flags2;
   };
   #pragma pack(pop)
}