#include "pch.h"
#include "include/Content/Content-Buffers/qgl_blender_buffer.h"

namespace qgl::graphics::content::buffers
{
   void swap(BLENDER_BUFFER& first,
             BLENDER_BUFFER& second) noexcept
   {
      using std::swap;
      swap(first.m_flags, second.m_flags);
      swap(first.m_blend_descs, second.m_blend_descs);
      swap(first.m_isAlphaToCoverage, second.m_isAlphaToCoverage);
      swap(first.m_isIndependentBlend, second.m_isIndependentBlend);
   }

   bool operator==(const BLENDER_BUFFER& r, const BLENDER_BUFFER& l) noexcept
   {
      bool intFieldsEqual = (r.m_flags == l.m_flags &&
         r.m_isAlphaToCoverage == l.m_isAlphaToCoverage &&
         r.m_isIndependentBlend == l.m_isIndependentBlend);

      return intFieldsEqual && (0 == MemoryCompare(r.blend_descs(),
                                                   l.blend_descs(),
                                                   NUM_RENDER_TARGETS));
   }

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

   BLENDER_BUFFER& BLENDER_BUFFER::operator=(BLENDER_BUFFER r) noexcept
   {
      swap(*this, r);
      return *this;
   }
}