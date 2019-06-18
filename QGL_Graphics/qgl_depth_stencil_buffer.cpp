#include "pch.h"
#include "include/Content/Content-Buffers/qgl_depth_stencil_buffer.h"

namespace qgl::content::buffers
{
   inline constexpr DEPTH_STENCIL_BUFFER::DEPTH_STENCIL_BUFFER() :
      Depth(),
      Format(DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT),
      Stencil(0),
      Flags1(0)
   {

   }
}