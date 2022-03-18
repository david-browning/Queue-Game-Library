#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/Content-Descriptors/qgl_engine_descriptor.h"

namespace qgl::graphics
{
   class device_configuration
   {
      public:
      constexpr device_configuration(
         const descriptors::engine_descriptor& desc) :
         m_buffers(desc.buffers),
         m_id(desc.pref_adapter_id),
         m_fullScreen(desc.fullscreen),
         m_hdr(desc.hdr),
         m_stereo(desc.stereo),
         m_tearing(desc.tearing),
         m_highRes(desc.high_resolution),
         m_width(desc.width),
         m_height(desc.height)
      {

      }

      device_configuration(const device_configuration&) = default;

      device_configuration(device_configuration&&) = default;

      ~device_configuration() noexcept = default;

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

      constexpr bool stereo() const noexcept
      {
         return m_stereo;
      }

      constexpr bool hdr() const noexcept
      {
         return m_hdr;
      }

      constexpr UINT adapter_devID() const noexcept
      {
         return m_id;
      }

      constexpr size_t buffers() const noexcept
      {
         return m_buffers;
      }

      constexpr size_t width() const noexcept
      {
         return m_width;
      }

      constexpr size_t height() const noexcept
      {
         return m_height;
      }

      friend void swap(
         device_configuration& l,
         device_configuration& r) noexcept
      {
         using std::swap;
         swap(l.m_width, r.m_width);
         swap(l.m_height, r.m_height);
         swap(l.m_buffers, r.m_buffers);
         swap(l.m_id, r.m_id);
         swap(l.m_fullScreen, r.m_fullScreen);
         swap(l.m_hdr, r.m_hdr);
         swap(l.m_stereo, r.m_stereo);
         swap(l.m_tearing, r.m_tearing);
         swap(l.m_highRes, r.m_highRes);
      }

      device_configuration& operator=(device_configuration r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      private:

      size_t m_width;
      size_t m_height;
      size_t m_buffers;
      UINT m_id;
      bool m_fullScreen;
      bool m_hdr;
      bool m_stereo;
      bool m_tearing;
      bool m_highRes;
   };
}