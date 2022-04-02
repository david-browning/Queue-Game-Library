#include "pch.h"
#include "include/Helpers/qgl_supported_helpers.h"

namespace qgl::graphics::helpers
{
   bool QGL_CC support_hdr(igpu_output* output_p) noexcept
   {
      DXGI_OUTPUT_DESC1 desc;
      if (FAILED(output_p->GetDesc1(&desc)))
      {
         return false;
      }
      return (desc.ColorSpace == DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020);
   }

   bool QGL_CC color_supported(DXGI_COLOR_SPACE_TYPE spc,
                               iswap_chain* sc_p) noexcept
   {
      UINT sptr = 0;
      if (FAILED(sc_p->CheckColorSpaceSupport(spc, &sptr)))
      {
         return false;
      }

      return (sptr & DXGI_SWAP_CHAIN_COLOR_SPACE_SUPPORT_FLAG_PRESENT) ==
         DXGI_SWAP_CHAIN_COLOR_SPACE_SUPPORT_FLAG_PRESENT;
   }

   bool QGL_CC support_tearing(igpu_factory* fac_p) noexcept
   {
      BOOL supported = false;
      if (FAILED(fac_p->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING,
                                            &supported,
                                            sizeof(supported))))
      {
         return false;
      }

      return static_cast<bool>(supported);
   }

   bool QGL_CC support_tiling(i3d_device* dev_p) noexcept
   {
      D3D12_FEATURE_DATA_D3D12_OPTIONS options = {};
      if (FAILED(dev_p->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS,
                                            &options,
                                            sizeof(options))))
      {
         return false;
      }

      return options.TiledResourcesTier !=
         D3D12_TILED_RESOURCES_TIER_NOT_SUPPORTED;
   }
}