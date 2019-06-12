#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::content::buffers
{
   #pragma pack(push, 1)
   struct QGL_GRAPHICS_API GRAPHICS_CONFIG_BUFFER final
   {
      public:
      /*
       Default constructor.
       Configured to support the largest number of displays.
       */
      GRAPHICS_CONFIG_BUFFER();

      /*
       Copy constructor.
       */
      GRAPHICS_CONFIG_BUFFER(const GRAPHICS_CONFIG_BUFFER&) = default;

      /* 
       Move constructor.
       */
      GRAPHICS_CONFIG_BUFFER(GRAPHICS_CONFIG_BUFFER&&) = default;

      /*
       Destructor. Do not mark as virtual.
       */
      ~GRAPHICS_CONFIG_BUFFER() noexcept = default;

      friend void swap(GRAPHICS_CONFIG_BUFFER& first, 
                       GRAPHICS_CONFIG_BUFFER& second) noexcept
      {
         using std::swap;
         swap(first.Flags, second.Flags);
         swap(first.Width, second.Width);
         swap(first.Height, second.Height);
         swap(first.Refresh, second.Refresh);
         swap(first.Buffers, second.Buffers);
         swap(first.IsFullScreen, second.IsFullScreen);
         swap(first.IsTearing, second.IsTearing);
         swap(first.IsHighResolution, second.IsHighResolution);
         swap(first.Is3D, second.Is3D);
         swap(first.IsHDR, second.IsHDR);
         swap(first.IsEnableInterlacing, second.IsEnableInterlacing);
         swap(first.PreferedAdapterDevID, second.PreferedAdapterDevID);
      }

      GRAPHICS_CONFIG_BUFFER& operator=(GRAPHICS_CONFIG_BUFFER r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      uint32_t Flags;
      uint32_t PreferedAdapterDevID;
      uint32_t Width;
      uint32_t Height;
      uint32_t Refresh;
      uint8_t Buffers;
      uint8_t IsFullScreen;
      uint8_t IsTearing;
      uint8_t IsHighResolution;
      uint8_t Is3D;
      uint8_t IsHDR;
      uint8_t IsEnableInterlacing;
   };
   #pragma pack(pop)
}