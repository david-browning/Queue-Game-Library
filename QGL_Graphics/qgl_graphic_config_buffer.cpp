#include "pch.h"
#include "include/Content/Content-Buffers/qgl_graphic_config_buffer.h"

namespace qgl::graphics::content::buffers
{
	GRAPHICS_CONFIG_BUFFER::GRAPHICS_CONFIG_BUFFER() :
      Flags(0),
      PreferedAdapterDevID(0),
      Width(640),
      Height(480),
      Refresh(60),
      Buffers(3),
      IsFullScreen(false),
      IsTearing(false),
      IsHighResolution(true),
      Is3D(false),
      IsHDR(false),
      IsEnableInterlacing(true)
	{

	}

   UINT GRAPHICS_CONFIG_BUFFER::width() const noexcept
   {
      return Width;
   }

   UINT GRAPHICS_CONFIG_BUFFER::height() const noexcept
   {
      return Height;
   }

   UINT GRAPHICS_CONFIG_BUFFER::refresh() const noexcept
   {
      return Refresh;
   }

   UINT GRAPHICS_CONFIG_BUFFER::buffers() const noexcept
   {
      return Buffers;
   }

   bool GRAPHICS_CONFIG_BUFFER::full_screen() const noexcept
   {
      return IsFullScreen;
   }

   bool GRAPHICS_CONFIG_BUFFER::tearing() const noexcept
   {
      return IsTearing;
   }

   bool GRAPHICS_CONFIG_BUFFER::high_resolution() const noexcept
   {
      return IsHighResolution;
   }

   bool GRAPHICS_CONFIG_BUFFER::stereo() const noexcept
   {
      return Is3D;
   }

   bool GRAPHICS_CONFIG_BUFFER::hdr() const noexcept
   {
      return IsHDR;
   }

   bool GRAPHICS_CONFIG_BUFFER::interlacing() const noexcept
   {
      return IsEnableInterlacing;
   }

   UINT GRAPHICS_CONFIG_BUFFER::adapter_devID() const noexcept
   {
      return PreferedAdapterDevID;
   }

}