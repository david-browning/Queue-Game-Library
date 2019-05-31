#include "pch.h"
#include "include/Content/Content-Buffers/qgl_graphic_config_buffer.h"

namespace qgl::graphics::content::buffers
{
	GRAPHICS_CONFIG_BUFFER::GRAPHICS_CONFIG_BUFFER()
	{
	}

   UINT GRAPHICS_CONFIG_BUFFER::width() const noexcept
   {
      return m_width;
   }

   UINT GRAPHICS_CONFIG_BUFFER::height() const noexcept
   {
      return m_height;
   }

   UINT GRAPHICS_CONFIG_BUFFER::refresh() const noexcept
   {
      return m_refresh;
   }

   UINT GRAPHICS_CONFIG_BUFFER::buffers() const noexcept
   {
      return m_buffers;
   }

   bool GRAPHICS_CONFIG_BUFFER::full_screen() const noexcept
   {
      return m_fullScreen;
   }

   bool GRAPHICS_CONFIG_BUFFER::tearing() const noexcept
   {
      return m_tearing;
   }

   bool GRAPHICS_CONFIG_BUFFER::high_resolution() const noexcept
   {
      return m_highResolution;
   }

   bool GRAPHICS_CONFIG_BUFFER::stereo() const noexcept
   {
      return m_3d;
   }

   bool GRAPHICS_CONFIG_BUFFER::hdr() const noexcept
   {
      return m_hdr;
   }

   bool GRAPHICS_CONFIG_BUFFER::interlacing() const noexcept
   {
      return m_enableInterlacing;
   }

   UINT GRAPHICS_CONFIG_BUFFER::adapter_devID() const noexcept
   {
      return m_preferedAdapterDevID;
   }

}