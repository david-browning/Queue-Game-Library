#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics::content::buffers
{
   struct QGL_GRAPHICS_API GRAPHICS_CONFIG_BUFFER
   {
      public:
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
       Returns true if the engine should allow an uncapped frame rate.
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
         swap(first.m_flags, second.m_flags);
         swap(first.m_width, second.m_width);
         swap(first.m_height, second.m_height);
         swap(first.m_refresh, second.m_refresh);
         swap(first.m_buffers, second.m_buffers);
         swap(first.m_fullScreen, second.m_fullScreen);
         swap(first.m_tearing, second.m_tearing);
         swap(first.m_highResolution, second.m_highResolution);
         swap(first.m_3d, second.m_3d);
         swap(first.m_hdr, second.m_hdr);
         swap(first.m_enableInterlacing, second.m_enableInterlacing);
         swap(first.m_preferedAdapterDevID, second.m_preferedAdapterDevID);
      }

      GRAPHICS_CONFIG_BUFFER& operator=(GRAPHICS_CONFIG_BUFFER r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      private:
      uint32_t m_flags;
      uint32_t m_preferedAdapterDevID;
      uint32_t m_width;
      uint32_t m_height;
      uint32_t m_refresh;
      uint8_t m_buffers;
      uint8_t m_fullScreen;
      uint8_t m_tearing;
      uint8_t m_highResolution;
      uint8_t m_3d;
      uint8_t m_hdr;
      uint8_t m_enableInterlacing;
   };
}