#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics::content::buffers
{
   struct QGL_GRAPHICS_API DEPTH_STENCIL_BUFFER
   {
      public:
      constexpr DEPTH_STENCIL_BUFFER();

      constexpr DEPTH_STENCIL_BUFFER(const DEPTH_STENCIL_BUFFER&) = default;

      constexpr DEPTH_STENCIL_BUFFER(DEPTH_STENCIL_BUFFER&&) = default;

      ~DEPTH_STENCIL_BUFFER() noexcept = default;

      DXGI_FORMAT format() const;

      const float& depth() const;

      float& depth();

      const uint8_t& stencil() const;

      uint8_t& stencil();

      friend void swap(DEPTH_STENCIL_BUFFER& first, 
                       DEPTH_STENCIL_BUFFER& second) noexcept
      {
         using std::swap;
         swap(first.m_depth, second.m_depth);
         swap(first.m_format, second.m_format);
         swap(first.m_stencil, second.m_stencil);
         swap(first.m_flags1, second.m_flags1);
      }

      DEPTH_STENCIL_BUFFER& operator=(DEPTH_STENCIL_BUFFER r)
      {
         swap(*this, r);
         return *this;
      }

      private:
      float m_depth;
      uint16_t m_format;
      uint8_t m_stencil;
      uint8_t m_flags1;
   };
}