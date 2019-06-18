#include "pch.h"
#include "include/Content/Content-Buffers/qgl_blender_buffer.h"

namespace qgl::content::buffers
{
   BLENDER_BUFFER::BLENDER_BUFFER() :
      Flags(DEFAULT_FLAGS),
      IsAlphaToCoverage(FALSE),
      IsIndependentBlend(FALSE)
   {
      //Array items are default constructed.
   }
}