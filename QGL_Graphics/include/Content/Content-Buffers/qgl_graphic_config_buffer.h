#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::content::buffers
{
   struct QGL_GRAPHICS_API GRAPHICS_CONFIG_BUFFER
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

      /*
       Returns the horizontal resolution in pixels.
      */
      UINT width() const noexcept;

      /*
       Returns the vertical resolution in pixels.
       */
      UINT height() const noexcept;

      /*
       Returns the monitor refresh rate in hertz.
       */
      UINT refresh() const noexcept;

      /*
       Returns the number of render target back buffers the engine will use.
       */
      UINT buffers() const noexcept;

      /*
       Returns true if the engine should be in full screen.
       */
      bool full_screen() const noexcept;
      /*
       Returns true if the engine should allow an uncapped render rate.
       */
      bool tearing() const noexcept;

      /*
       Returns true if the engine uses high resolution rendering.
       */
      bool high_resolution() const noexcept;

      bool stereo() const noexcept;

      bool hdr() const noexcept;

      bool interlacing() const noexcept;

      UINT adapter_devID() const noexcept;

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
}