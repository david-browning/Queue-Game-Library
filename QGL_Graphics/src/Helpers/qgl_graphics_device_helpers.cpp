
#include "pch.h"
#include "include/Helpers/qgl_window_helpers.h"

namespace qgl::graphics::helpers
{
   bool support_tearing(dxgi_factory* fac_p)
   {
      BOOL supported = false;
      winrt::check_hresult(fac_p->CheckFeatureSupport(
         DXGI_FEATURE_PRESENT_ALLOW_TEARING,
         &supported,
         sizeof(supported)));

      return static_cast<bool>(supported);
   }

   bool support_hdr(IDXGIOutput6* output_p)
   {
      DXGI_OUTPUT_DESC1 desc;
      winrt::check_hresult(output_p->GetDesc1(&desc));
      return (desc.ColorSpace == DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020);
   }

   bool support_stereo()
   {
      throw winrt::hresult_not_implemented{};
   }
}