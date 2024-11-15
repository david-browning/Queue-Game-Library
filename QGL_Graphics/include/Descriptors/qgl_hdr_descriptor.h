#pragma once
#include "include/qgl_graphics_include.h"
#include <QGLStruct.h>
#include <QGLMemory.h>

namespace qgl::descriptors
{
   enum class hdr_modes :uint8_t
   {
      none = DXGI_HDR_METADATA_TYPE_NONE,
      hdr10 = DXGI_HDR_METADATA_TYPE_HDR10,
      hdr10_plus = DXGI_HDR_METADATA_TYPE_HDR10PLUS,
   };

#pragma pack(push, 1)
   struct hdr_descriptor final
   {
      /*
       Default is no HDR
       */
      constexpr hdr_descriptor()
      {

      }

      constexpr hdr_descriptor(const hdr_descriptor&) = default;

      constexpr hdr_descriptor(hdr_descriptor&&) noexcept = default;

      ~hdr_descriptor() noexcept = default;

      friend void swap(hdr_descriptor& l, hdr_descriptor& r) noexcept
      {
         using std::swap;
         swap(l.b_primary, r.b_primary);
         swap(l.g_primary, r.g_primary);
         swap(l.light_avg, r.light_avg);
         swap(l.light_max, r.light_max);
         swap(l.lum_max, r.lum_max);
         swap(l.lum_min, r.lum_min);
         swap(l.mode, r.mode);
         swap(l.reserved1, r.reserved1);
         swap(l.reserved2, r.reserved2);
         swap(l.r_primary, r.r_primary);
         swap(l.white, r.white);
      }

      hdr_descriptor& operator=(hdr_descriptor r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      /*
       The maximum number of nits of the display used to master the content.
       Values are in whole nits.
       */
      uint32_t lum_max = 0;

      /*
       The minimum number of nits of the display used to master the content.
       Values are 1/10000th of a nit (0.0001 nit).
       */
      uint32_t lum_min = 0;

      /*
       The chromaticity coordinates of the red value in the CIE1931 color
       space. Index 0 contains the X coordinate and index 1 contains the Y
       coordinate. The values are normalized to 50,000.
       */
      fixed_buffer<uint16_t, 2> r_primary;

      /*
       The chromaticity coordinates of the green value in the CIE1931 color
       space. Index 0 contains the X coordinate and index 1 contains the Y
       coordinate. The values are normalized to 50,000.
       */
      fixed_buffer<uint16_t, 2> g_primary;

      /*
       The chromaticity coordinates of the blue value in the CIE1931 color
       space. Index 0 contains the X coordinate and index 1 contains the Y
       coordinate. The values are normalized to 50,000.
       */
      fixed_buffer<uint16_t, 2> b_primary;

      /*
       The chromaticity coordinates of the white point in the CIE1931 color
       space. Index 0 contains the X coordinate and index 1 contains the Y
       coordinate. The values are normalized to 50,000.
       */
      fixed_buffer<uint16_t, 2> white;

      /*
       The maximum content light level (MaxCLL). This is the nit value
       corresponding to the brightest pixel used anywhere in the content.
       */
      uint16_t light_max = 0;

      /*
       The maximum frame average light level (MaxFALL). This is the nit value
       corresponding to the average luminance of the frame which has the
       brightest average luminance anywhere in the content.
       */
      uint16_t light_avg = 0;

      hdr_modes mode = hdr_modes::none;

      uint8_t reserved1 = 0;

      uint16_t reserved2 = 0;
   };
#pragma pack(pop)
}