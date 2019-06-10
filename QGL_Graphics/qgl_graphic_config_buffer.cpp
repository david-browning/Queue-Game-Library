#include "pch.h"
#include "include/Content/Content-Buffers/qgl_graphic_config_buffer.h"

namespace qgl::content::buffers
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
}