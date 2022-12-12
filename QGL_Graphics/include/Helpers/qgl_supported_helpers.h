#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics::helpers
{
   // TODO: if lots of features need to be queried, use this helper:
   // CD3DX12FeatureSupport
   // https://devblogs.microsoft.com/directx/introducing-a-new-api-for-checking-feature-support-in-direct3d-12/

   /*
    Returns true if the monitor supports HDR.
    HDR information is included in "desc".
    */
   extern "C" QGL_GRAPHICS_API bool QGL_CC support_hdr(
      igpu_output* output_p) noexcept;

   extern "C" QGL_GRAPHICS_API bool QGL_CC color_supported(
      DXGI_COLOR_SPACE_TYPE spc, 
      iswap_chain* sc_p) noexcept;

   extern "C" QGL_GRAPHICS_API bool QGL_CC support_tiling(
      i3d_device* dev_p) noexcept;

   /*
    Returns true if the GPU factory supports tearing on the monitor.
    */
   extern "C" QGL_GRAPHICS_API bool QGL_CC support_tearing(
      igpu_factory* fac_p) noexcept;
}