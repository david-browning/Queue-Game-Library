#include "pch.h"
#include "include/Content/Content-Buffers/qgl_depth_stencil_buffer.h"

namespace qgl::content::buffers
{
   inline constexpr DEPTH_STENCIL_BUFFER::DEPTH_STENCIL_BUFFER() :
      Depth(0),
      Format(DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT),
      Stencil(0),
      Flags1(0)
   {

   }

   DXGI_FORMAT DEPTH_STENCIL_BUFFER::format() const
   {
      return static_cast<DXGI_FORMAT>(Format);
   }

   const float& DEPTH_STENCIL_BUFFER::depth() const
   {
      return Depth;
   }

   float& DEPTH_STENCIL_BUFFER::depth()
   {
      return Depth;
   }

   const uint8_t& DEPTH_STENCIL_BUFFER::stencil() const
   {
      return Stencil;
   }

   uint8_t& DEPTH_STENCIL_BUFFER::stencil()
   {
      return Stencil;
   }
}