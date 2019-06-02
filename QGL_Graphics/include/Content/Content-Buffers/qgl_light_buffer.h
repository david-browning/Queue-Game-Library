#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::content::buffers
{
   struct QGL_GRAPHICS_API LIGHT_BUFFER
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

      /*
       Returns a pointer to an array of 4 floats. Each float represents the
       R, G, B, and A color values. Values must be between 0 and 1.0.
       */
      const float* color() const noexcept;

      /*
       Returns a pointer to an array of 4 floats. Each float represents the 
       X, Y, and Z components. The last value is unused.
       */
      const float* look_at() const noexcept;

      /*
       Returns a pointer to an array of 4 floats. Each float represents the 
       X, Y, and Z components. The last value is unused.
       */
      const float* position() const noexcept;

      /*
       Returns the light intensity. Value must be between 0 and 1.0;
       */
      float intensity() const noexcept;

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

      float Color[4];
      float LookAt[4];
      float Position[4];
      float Intensity;
      uint16_t Flags1;
      uint8_t Flags2;
   };
}