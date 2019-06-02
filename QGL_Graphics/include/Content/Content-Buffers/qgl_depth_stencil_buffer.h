#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::content::buffers
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
         swap(first.Depth, second.Depth);
         swap(first.Format, second.Format);
         swap(first.Stencil, second.Stencil);
         swap(first.Flags1, second.Flags1);
      }

      DEPTH_STENCIL_BUFFER& operator=(DEPTH_STENCIL_BUFFER r)
      {
         swap(*this, r);
         return *this;
      }

      float Depth;
      uint16_t Format;
      uint8_t Stencil;
      uint8_t Flags1;
   };
}