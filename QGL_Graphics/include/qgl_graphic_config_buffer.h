#pragma once
#include "qgl_graphics_include.h"

namespace qgl::graphics
{
   struct LIB_EXPORT GRAPHICS_CONFIG_BUFFER
   {
      public:
      GRAPHICS_CONFIG_BUFFER();

      GRAPHICS_CONFIG_BUFFER(const GRAPHICS_CONFIG_BUFFER& r);

      GRAPHICS_CONFIG_BUFFER(GRAPHICS_CONFIG_BUFFER&& r);

      ~GRAPHICS_CONFIG_BUFFER() = default;

      /*
       Returns the horizontal resolution in pixels.
      */
      inline UINT width() const
      {
         return m_width;
      }

      inline void width(UINT w)
      {
         m_width = w;
      }

      /*
       Returns the vertical resolution in pixels.
       */
      inline UINT height() const
      {
         return m_height;
      }

      inline void height(UINT h)
      {
         m_height = h;
      }

      /*
       Returns the monitor refresh rate in hertz.
       */
      inline UINT refresh() const
      {
         return m_refresh;
      }

      inline void refresh(UINT r)
      {
        m_refresh = r;
      }

      /*
       Returns the number of render target back buffers the engine will use.
       */
      inline UINT buffers() const
      {
         return m_buffers;
      }

      inline void buffers(uint8_t b)
      {
         m_buffers = b;
      }

      /*
       Returns true if the engine should be in full screen.
       */
      inline bool full_screen() const
      {
         return m_fullScreen;
      }

      inline void full_screen(bool enable)
      {
         m_fullScreen = enable;
      }

      /*
       Returns true if the engine should allow an uncapped frame rate.
       */
      inline bool tearing() const
      {
         return m_tearing;
      }

      inline void tearing(bool enable)
      {
         m_tearing = enable;
      }

      /*
       Returns true if the engine uses high resolution rendering.
       */
      inline bool high_resolution() const
      {
         return m_highResolution;
      }

      inline void high_resolution(bool enable)
      {
         m_highResolution = enable;
      }

      inline bool stereo() const
      {
         return m_3d;
      }

      inline void stereo(bool enable)
      {
         m_3d = enable;
      }

      inline bool hdr() const
      {
         return m_hdr;
      }

      inline void hdr(bool enable)
      {
         m_hdr = enable;
      }

      inline bool interlacing() const
      {
         return m_enableInterlacing;
      }

      inline void interlacing(bool enable)
      {
         m_enableInterlacing = enable;
      }

      inline UINT adapter_devID() const
      {
         return m_preferedAdapterDevID;
      }

      inline void adapter_devID(UINT devID)
      {
         m_preferedAdapterDevID = devID;
      }

      friend void swap(GRAPHICS_CONFIG_BUFFER& first, GRAPHICS_CONFIG_BUFFER& second) noexcept
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