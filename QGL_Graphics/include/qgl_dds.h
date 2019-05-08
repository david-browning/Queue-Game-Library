#pragma once
#include "qgl_graphics_include.h"
#include "qgl_texture_buffer.h"

namespace qgl::graphics::low
{
   enum DDS_ALPHA_MODE
   {
      DDS_ALPHA_MODE_UNKNOWN = 0,
      DDS_ALPHA_MODE_STRAIGHT = 1,
      DDS_ALPHA_MODE_PREMULTIPLIED = 2,
      DDS_ALPHA_MODE_OPAQUE = 3,
      DDS_ALPHA_MODE_CUSTOM = 4,
   };

   extern LIB_EXPORT D3D12_SRV_DIMENSION srv_dim_from_tbuffer(const TEXTURE_BUFFER& buff);

   /*
    Fills a texture buffer using the raw texture data.
    */
   extern LIB_EXPORT void load_texture_data(const winrt::com_ptr<d3d_device>& dev_p,
                                            TEXTURE_BUFFER* const data_p);

}
