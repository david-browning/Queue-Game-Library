#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::content::buffers
{
   #pragma pack(push, 1)
   /*
    Describes the parameters for Z-Buffering.
    */
   struct QGL_GRAPHICS_API DEPTH_STENCIL_BUFFER final
   {
      public:
      /*
       Creates default depth stencil parameters.
       */
      constexpr DEPTH_STENCIL_BUFFER();

      /*
       Copy constructor.
       */
      constexpr DEPTH_STENCIL_BUFFER(const DEPTH_STENCIL_BUFFER&) = default;

      /*
       Move constructor.
       */
      constexpr DEPTH_STENCIL_BUFFER(DEPTH_STENCIL_BUFFER&&) = default;

      /*
       Destructor. Do not mark virtual.
       */
      ~DEPTH_STENCIL_BUFFER() noexcept = default;

      friend void swap(DEPTH_STENCIL_BUFFER& first, 
                       DEPTH_STENCIL_BUFFER& second) noexcept
      {
         using std::swap;
         swap(first.Depth, second.Depth);
         swap(first.Format, second.Format);
         swap(first.Stencil, second.Stencil);
         swap(first.Flags1, second.Flags1);
      }

      DEPTH_STENCIL_BUFFER& operator=(DEPTH_STENCIL_BUFFER r)
      {
         swap(*this, r);
         return *this;
      }

      /*
       This is the value to clear the depth buffer with. It must be between 0 
       and 1.
       Default value is 0.
       */
      math::rational<int32_t> Depth;

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
      uint16_t Format;

      /*
       When clearing the stencil buffer, this is the value that gets used.
       Default value is 0.
       */
      uint8_t Stencil;

      /*
       Currently unused.
       */
      uint8_t Flags1;
   };
   #pragma pack(pop)
}