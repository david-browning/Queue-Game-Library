#include "pch.h"
#include "include/Content/Content-Buffers/qgl_blender_buffer.h"

namespace qgl::graphics::content::buffers
{
   BLENDER_BUFFER::BLENDER_BUFFER() :
      m_flags(DEFAULT_FLAGS),
      m_isAlphaToCoverage(FALSE),
      m_isIndependentBlend(FALSE)
   {
      //Array items are default constructed.
   }

   bool BLENDER_BUFFER::alpha_coverage() const
   {
      return static_cast<bool>(m_isAlphaToCoverage);
   }

   bool BLENDER_BUFFER::independent_blend() const
   {
      return static_cast<bool>(m_isIndependentBlend);
   }

   const BLEND_DESC* BLENDER_BUFFER::blend_descs() const
   {
      return m_blend_descs;
   }
}