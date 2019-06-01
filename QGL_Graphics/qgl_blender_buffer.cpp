#include "pch.h"
#include "include/Content/Content-Buffers/qgl_blender_buffer.h"

namespace qgl::graphics::content::buffers
{
   BLENDER_BUFFER::BLENDER_BUFFER() :
      Flags(DEFAULT_FLAGS),
      IsAlphaToCoverage(FALSE),
      IsIndependentBlend(FALSE)
   {
      //Array items are default constructed.
   }

   bool BLENDER_BUFFER::alpha_coverage() const
   {
      return static_cast<bool>(IsAlphaToCoverage);
   }

   bool BLENDER_BUFFER::independent_blend() const
   {
      return static_cast<bool>(IsIndependentBlend);
   }

   const BLEND_DESC* BLENDER_BUFFER::blend_descs() const
   {
      return BlendDescriptions;
   }
}