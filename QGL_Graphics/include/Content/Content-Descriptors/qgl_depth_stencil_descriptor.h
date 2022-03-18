#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics::descriptors
{
#pragma pack(push, 1)
   /*
    Describes the parameters for Z-Buffering.
    */
   struct depth_stencil_descriptor final
   {
      public:
      /*
       Creates default depth stencil parameters.
       */
      constexpr depth_stencil_descriptor() :
         depth(),
         format(DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT),
         stencil(0),
         flags1(0)
      {
      }

      depth_stencil_descriptor(const depth_stencil_descriptor&) = default;

      depth_stencil_descriptor(depth_stencil_descriptor&&) = default;

      ~depth_stencil_descriptor() noexcept = default;

      friend void swap(depth_stencil_descriptor& l,
         depth_stencil_descriptor& r) noexcept
      {
         using std::swap;
         swap(l.depth, r.depth);
         swap(l.format, r.format);
         swap(l.stencil, r.stencil);
         swap(l.flags1, r.flags1);
      }

      depth_stencil_descriptor& operator=(depth_stencil_descriptor r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      /*
       This is the value to clear the depth buffer with. It must be between 0
       and 1.
       Default value is 0.
       */
      math::rational<int32_t> depth;

      /*
       DXGI_FORMAT format to use for the depth stencil texture.
       Default is a single, 32-bit float, component. The only supported values
       are:
         DXGI_FORMAT_D16_UNORM
         DXGI_FORMAT_D24_UNORM_S8_UINT
         DXGI_FORMAT_D32_FLOAT
         DXGI_FORMAT_D32_FLOAT_S8X24_UINT
       QGL does not verify a that a valid format is set!
       */
      uint16_t format;

      /*
       When clearing the stencil buffer, this is the value that gets used.
       Default value is 0.
       */
      uint8_t stencil;

      /*
       Currently unused.
       */
      uint8_t flags1;
   };
#pragma pack(pop)
}