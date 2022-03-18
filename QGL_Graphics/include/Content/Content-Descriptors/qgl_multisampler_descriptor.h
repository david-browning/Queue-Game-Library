#pragma once
#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics::descriptors
{
   //https://docs.microsoft.com/en-us/windows/win32/api/dxgicommon/ns-dxgicommon-dxgi_sample_desc
#pragma pack(push, 1)
   struct multisampler_descriptor final
   {
      public:
      constexpr multisampler_descriptor() :
         count(1),
         quality(0),
         reserved1(0)
      {
      }

      multisampler_descriptor(const multisampler_descriptor&) = default;

      multisampler_descriptor(multisampler_descriptor&&) = default;

      ~multisampler_descriptor() noexcept = default;

      friend void swap(multisampler_descriptor& l,
         multisampler_descriptor& r) noexcept
      {
         using std::swap;
         swap(l.count, r.count);
         swap(l.quality, r.quality);
         swap(l.reserved1, r.reserved1);
      }

      multisampler_descriptor& operator=(multisampler_descriptor r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      /*
       The number of multisamples per pixel.
       Default count is 1.
       */
      uint16_t count;

      /*
       The image quality level.
       Default is 0.
       */
      uint16_t quality;
      uint32_t reserved1;
   };
#pragma pack(pop)
}