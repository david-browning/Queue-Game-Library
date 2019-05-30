#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics::content::buffers
{
   struct QGL_GRAPHICS_API LIGHT_BUFFER
   {
      public:
      LIGHT_BUFFER();

      LIGHT_BUFFER(const LIGHT_BUFFER& r);

      LIGHT_BUFFER(LIGHT_BUFFER&& r);

      ~LIGHT_BUFFER() noexcept;

      const float* color() const
      {
         return m_color;
      }

      const float* look_at() const
      {
         return m_lookAt;
      }

      const float* position() const
      {
         return m_position;
      }

      float intensity() const
      {
         return m_intensity;
      }

      uint16_t flags1() const
      {
         return m_flags1;
      }

      uint8_t flags2() const
      {
         return m_flags2;
      }

      private:
      float m_color[4];
      float m_lookAt[4];
      float m_position[4];
      float m_intensity;
      uint16_t m_flags1;
      uint8_t m_flags2;
   };
}