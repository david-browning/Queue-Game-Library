#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/Content-Descriptors/qgl_hdr_descriptor.h"
#include "include/Content/Content-Descriptors/qgl_engine_descriptor.h"

namespace qgl::graphics
{
   class gpu_config
   {
      public:
      constexpr gpu_config(const descriptors::engine_descriptor& desc,
                           descriptors::hdr_descriptor&& hdrDesc) :
         m_hdr(std::forward<descriptors::hdr_descriptor>(hdrDesc)),
         m_buffers(desc.buffers),
         m_id(desc.pref_adapter_id),
         m_fullScreen(desc.fullscreen),
         m_tearing(desc.tearing),
         m_highRes(desc.high_resolution),
         m_width(desc.width),
         m_height(desc.height),
         m_console(desc.console),
         m_textMode(static_cast<DWRITE_RENDERING_MODE>(desc.text_mode))
      {

      }

      constexpr bool full_screen() const noexcept
      {
         return m_fullScreen;
      }

      constexpr bool tearing() const noexcept
      {
         return m_tearing;
      }

      constexpr bool high_resolution() const noexcept
      {
         return m_highRes;
      }

      constexpr hdr_modes hdr_mode() const noexcept
      {
         return m_hdr.mode;
      }

      constexpr UINT adapter_devID() const noexcept
      {
         return m_id;
      }

      constexpr size_t buffers() const noexcept
      {
         return m_buffers;
      }

      /*
       Render target width (in physical pixels). This gets transformed to 
       device independent pixels later.
       */
      constexpr physp_t width() const noexcept
      {
         return m_width;
      }

      /*
       Render target height (in physical pixels). This gets transformed to
       device independent pixels later.
       */
      constexpr physp_t height() const noexcept
      {
         return m_height;
      }

      constexpr DWRITE_RENDERING_MODE text_mode() const noexcept
      {
         return m_textMode;
      }

      constexpr bool console() const noexcept
      {
         return m_console;
      }

      DXGI_HDR_METADATA_HDR10 hdr10() const
      {
         if (m_hdr.mode != hdr_modes::hdr10)
         {
            throw std::bad_optional_access{};
         }

         DXGI_HDR_METADATA_HDR10 ret = {};
         ret.RedPrimary[0] = m_hdr.r_primary[0];
         ret.RedPrimary[1] = m_hdr.r_primary[1];
         ret.GreenPrimary[0] = m_hdr.g_primary[0];
         ret.GreenPrimary[1] = m_hdr.g_primary[1];
         ret.BluePrimary[0] = m_hdr.b_primary[0];
         ret.BluePrimary[1] = m_hdr.b_primary[1];
         ret.WhitePoint[0] = m_hdr.white[0];
         ret.WhitePoint[1] = m_hdr.white[1];
         ret.MaxMasteringLuminance = m_hdr.lum_max;
         ret.MinMasteringLuminance = m_hdr.lum_min;
         ret.MaxContentLightLevel = m_hdr.light_max;
         ret.MaxFrameAverageLightLevel = m_hdr.light_avg;

         return ret;
      }

      DXGI_HDR_METADATA_HDR10PLUS hdr10_plus() const
      {
         throw std::bad_optional_access{};
      }

      friend void swap(
         gpu_config& l,
         gpu_config& r) noexcept
      {
         using std::swap;
         swap(l.m_width, r.m_width);
         swap(l.m_height, r.m_height);
         swap(l.m_buffers, r.m_buffers);
         swap(l.m_id, r.m_id);
         swap(l.m_fullScreen, r.m_fullScreen);
         swap(l.m_tearing, r.m_tearing);
         swap(l.m_highRes, r.m_highRes);
         swap(l.m_hdr, r.m_hdr);
         swap(l.m_console, r.m_console);
      }

      gpu_config& operator=(gpu_config r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      private:

      descriptors::hdr_descriptor m_hdr;
      physp_t m_width;
      physp_t m_height;
      size_t m_buffers;
      UINT m_id;
      DWRITE_RENDERING_MODE m_textMode;
      bool m_fullScreen;
      bool m_tearing;
      bool m_highRes;
      bool m_console;
   };
}