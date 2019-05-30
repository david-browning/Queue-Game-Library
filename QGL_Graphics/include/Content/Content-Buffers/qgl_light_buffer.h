#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics::content::buffers
{
   struct QGL_GRAPHICS_API LIGHT_BUFFER
   {
      public:
      /*
       Default Constructor
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
      const float* color() const;

      /*
       Returns a pointer to an array of 4 floats. Each float represents the 
       X, Y, and Z components. The last value is unused.
       */
      const float* look_at() const;

      /*
       Returns a pointer to an array of 4 floats. Each float represents the 
       X, Y, and Z components. The last value is unused.
       */
      const float* position() const;

      /*
       Returns the light intensity. Value must be between 0 and 1.0;
       */
      float& intensity();

      friend void swap(LIGHT_BUFFER& l,
                       LIGHT_BUFFER& r) noexcept;

      LIGHT_BUFFER& operator=(LIGHT_BUFFER r) noexcept;

      private:
      float m_color[4];
      float m_lookAt[4];
      float m_position[4];
      float m_intensity;
      uint16_t m_flags1;
      uint8_t m_flags2;
   };
}