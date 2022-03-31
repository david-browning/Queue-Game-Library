#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/qgl_device_configuration.h"

namespace qgl::graphics::helpers
{
   /*
    Returns true if the monitor supports HDR.
    HDR information is included in "desc".
    */
   extern QGL_GRAPHICS_API bool support_hdr(igpu_output* output_p);

   extern QGL_GRAPHICS_API DXGI_FORMAT color_format(hdr_modes mode) noexcept;

   extern QGL_GRAPHICS_API DXGI_COLOR_SPACE_TYPE color_space(
      hdr_modes mode) noexcept;

   extern QGL_GRAPHICS_API bool color_supported(
      DXGI_COLOR_SPACE_TYPE spc, iswap_chain* sc_p);


   inline void set_hdr(const gpu_config& config, iswap_chain* sc_p)
   {
      switch (config.hdr_mode())
      {
         case hdr_modes::hdr10:
         {
            auto data = config.hdr10();
            sc_p->SetHDRMetaData(DXGI_HDR_METADATA_TYPE_HDR10,
               sizeof(DXGI_HDR_METADATA_HDR10), &data);
            break;
         }
         case hdr_modes::hdr10_plus:
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