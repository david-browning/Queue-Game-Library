#include "pch.h"
#include "include/Content/Content-Buffers/qgl_multisample_desc_buffer.h"

namespace qgl::content::buffers
{
   MULTISAMPLE_DESC_BUFFER::MULTISAMPLE_DESC_BUFFER(uint16_t count, 
                                                    uint16_t quality) :
      Count(count),
      Quality(quality),
      Reserved1(0)
   {
   }
}