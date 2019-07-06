#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/Content-Buffers/qgl_blender_buffer.h"

namespace qgl::graphics::gpu::render
{
   /*
    The blender provides the parameters describing how to blend pixels.
    */
   class QGL_GRAPHICS_API blender
   {
      public:
      /*
       Constructs a blend using the given buffer.
       */
      blender(const qgl::content::buffers::BLENDER_BUFFER* buff);

      /*
       Copy constructor.
       */
      blender(const blender&) = default;

      /*
       Move constructor.
       */
      blender(blender&&) = default;

      /*
       Destructor.
       */
      ~blender() noexcept = default;

      /*
       Returns a description of the blender.
       Used by a pipeline state object.
       */
      const D3D12_BLEND_DESC* description() const;

      /*
       The sample mask for the blend state.
       Used by a pipeline state object.
       */
      UINT mask() const noexcept;

      const float* blend_factor() const noexcept;

      /*
       Swaps the contents of l and r.
       */
      friend void swap(blender& l, blender& r) noexcept
      {
         using std::swap;
         swap(l.m_desc, r.m_desc);
         swap(l.m_mask, r.m_mask);
         swap(l.m_blendFactor, r.m_blendFactor);
      }

      /*
       Assignment Operator
       */
      blender& operator=(blender r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      private:
      D3D12_BLEND_DESC m_desc;
      UINT m_mask;
      float m_blendFactor[4];
   };
}