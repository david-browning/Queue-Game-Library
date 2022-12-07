#pragma once
#include "include/qgl_graphics_include.h"
#include "include/qgl_device_configuration.h"

namespace qgl::graphics::helpers
{
   /*
    Looks up the color format from an hdr_modes.
    */
   extern "C" QGL_GRAPHICS_API DXGI_FORMAT color_format(
      descriptors::hdr_modes mode) noexcept;

   /*
    Looks up the color space from an hdr_modes.
    */
   extern "C" QGL_GRAPHICS_API DXGI_COLOR_SPACE_TYPE color_space(
      descriptors::hdr_modes mode) noexcept;

   /*
    Sets the HDR mode for the given swap chain.
    */
   inline void set_hdr(const gpu_config& config, iswap_chain* sc_p)
   {
      switch (config.hdr_mode())
      {
         case descriptors::hdr_modes::hdr10:
         {
            auto data = config.hdr10();
            sc_p->SetHDRMetaData(DXGI_HDR_METADATA_TYPE_HDR10,
               sizeof(DXGI_HDR_METADATA_HDR10), &data);
            break;
         }
         case descriptors::hdr_modes::hdr10_plus:
         {
            auto data = config.hdr10_plus();
            sc_p->SetHDRMetaData(DXGI_HDR_METADATA_TYPE_HDR10PLUS,
               sizeof(DXGI_HDR_METADATA_HDR10PLUS), &data);
            break;
         }
         //Do nothing on default.
      }
   }
}