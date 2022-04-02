
#include "pch.h"
#include "include/Helpers/qgl_graphics_device_helpers.h"

namespace qgl::graphics::helpers
{
   size_t format_size(DXGI_FORMAT f) noexcept
   {
      return DirectX::BitsPerPixel(f) / size_t(8);
   }
}