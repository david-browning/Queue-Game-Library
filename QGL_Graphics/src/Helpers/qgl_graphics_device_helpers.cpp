
#include "pch.h"
#include "include/Helpers/qgl_graphics_device_helpers.h"

namespace qgl::graphics::helpers
{
   bool support_tearing(factory_gpu* fac_p)
   {
      BOOL supported = false;
      winrt::check_hresult(fac_p->CheckFeatureSupport(
         DXGI_FEATURE_PRESENT_ALLOW_TEARING,
         &supported,
         sizeof(supported)));

      return static_cast<bool>(supported);
   }

   bool support_stereo()
   {
      throw winrt::hresult_not_implemented{};
   }

   bool support_tiling(device_3d* dev_p) noexcept
   {
      D3D12_FEATURE_DATA_D3D12_OPTIONS options = {};
      dev_p->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS,
         &options, sizeof(options));

      return options.TiledResourcesTier !=
         D3D12_TILED_RESOURCES_TIER_NOT_SUPPORTED;
   }

}