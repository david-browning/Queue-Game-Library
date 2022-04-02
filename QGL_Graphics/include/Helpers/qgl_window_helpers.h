#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics::helpers
{
   /*
    Converts device-independent pixels to physical pixels.
    */
   constexpr physp_t dip_to_pixels(dip_t dip, dpi_t dpi)
   {
      return static_cast<physp_t>(dip * dpi / 96.0f);
   }

   /*
    Converts physical pixels to device-independent pixels.
    */
   constexpr dip_t pixels_to_dip(physp_t pxls, dpi_t dpi)
   {
      return static_cast<dip_t>(pxls) * 96.0f / dpi;
   }
}