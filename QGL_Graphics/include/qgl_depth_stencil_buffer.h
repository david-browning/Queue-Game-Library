#pragma once
#include "qgl_graphics_include.h"

namespace qgl::graphics
{
   struct LIB_EXPORT DEPTH_STENCIL_BUFFER
   {
      public:

      constexpr DEPTH_STENCIL_BUFFER();

      constexpr DEPTH_STENCIL_BUFFER(const DEPTH_STENCIL_BUFFER& r);

      constexpr DEPTH_STENCIL_BUFFER(DEPTH_STENCIL_BUFFER&& r);

      inline DXGI_FORMAT format() const
      {
         return static_cast<DXGI_FORMAT>(m_format);
      }

      inline float depth() const
      {
         return m_depth;
      }

      inline uint8_t stencil() const
      {
         return m_stencil;
      }

      friend void swap(DEPTH_STENCIL_BUFFER& first, DEPTH_STENCIL_BUFFER& second) noexcept
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