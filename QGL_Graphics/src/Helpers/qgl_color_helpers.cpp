#include "pch.h"
#include "include/Helpers/qgl_color_helpers.h"

namespace qgl::graphics::helpers
{
   struct hdr_lookup_data
   {
      constexpr hdr_lookup_data(DXGI_FORMAT sFmt, DXGI_COLOR_SPACE_TYPE col) :
         swapchain_fmt(sFmt), color_space(col)
      {

      }

      DXGI_FORMAT swapchain_fmt;
      DXGI_COLOR_SPACE_TYPE color_space;
   };

   static const std::unordered_map<hdr_modes, hdr_lookup_data> HDR_LOOKUP =
   {
      { hdr_modes::none, {
         DXGI_FORMAT_R8G8B8A8_UNORM,
         DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709} },
      { hdr_modes::hdr10, {
         DXGI_FORMAT_R10G10B10A2_UNORM,
         DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020} },
      { hdr_modes::hdr10_plus, {
         DXGI_FORMAT_R16G16B16A16_FLOAT,
         DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709} }
   };

   DXGI_FORMAT color_format(hdr_modes mode) noexcept
   {
      return HDR_LOOKUP.at(mode).swapchain_fmt;
   }

   DXGI_COLOR_SPACE_TYPE color_space(hdr_modes mode) noexcept
   {
      return HDR_LOOKUP.at(mode).color_space;
   }
}
