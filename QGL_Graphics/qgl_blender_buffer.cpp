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

   bool BLENDER_BUFFER::alpha_coverage() const noexcept
   {
      return static_cast<bool>(IsAlphaToCoverage);
   }

   bool BLENDER_BUFFER::independent_blend() const noexcept
   {
      return static_cast<bool>(IsIndependentBlend);
   }

   const float* BLENDER_BUFFER::blend_factor() const noexcept
   {
      return BlendFactor;
   }

   const BLEND_DESC* BLENDER_BUFFER::blend_descs() const noexcept
   {
      return BlendDescriptions;
   }
}