#include "pch.h"
#include "include/Content/qgl_graphics_config.h"

namespace qgl::content
{
   graphics_config::graphics_config(const buffers::GRAPHICS_CONFIG_BUFFER* b, 
                                    const wchar_t* name, 
                                    content_id id) :
      m_buffer(*b),
      content_item(name, 
                   id,
                   RESOURCE_TYPE_DESCRIPTION,
                   CONTENT_LOADER_ID_GRAPHICS_CONFIG)
   {
   }

   UINT graphics_config::width() const noexcept
   {
      return m_buffer.Width;
   }

   UINT graphics_config::height() const noexcept
   {
      return m_buffer.Height;
   }

   UINT graphics_config::refresh() const noexcept
   {
      return m_buffer.Refresh;
   }

   UINT graphics_config::buffers() const noexcept
   {
      return m_buffer.Buffers;
   }

   bool graphics_config::full_screen() const noexcept
   {
      return m_buffer.IsFullScreen;
   }

   bool graphics_config::tearing() const noexcept
   {
      return m_buffer.IsTearing;
   }

   bool graphics_config::high_resolution() const noexcept
   {
      return m_buffer.IsHighResolution;
   }

   bool graphics_config::stereo() const noexcept
   {
      return m_buffer.Is3D;
   }

   bool graphics_config::hdr() const noexcept
   {
      return m_buffer.IsHDR;
   }

   bool graphics_config::interlacing() const noexcept
   {
      return m_buffer.IsEnableInterlacing;
   }

   UINT graphics_config::adapter_devID() const noexcept
   {
      return m_buffer.PreferedAdapterDevID;
   }
}