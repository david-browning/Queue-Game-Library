#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/Content-Descriptors/qgl_blender_descriptor.h"

namespace qgl::graphics::gpu
{
   /*
    The blender provides the parameters describing how to blend pixels.
    */
   class blender
   {
      public:
      /*
       Constructs a blend using the given buffer.
       */
      blender(const descriptors::blender_descriptor& buff) :
         m_desc(),
         m_mask(UINT_MAX)
      {
         m_desc.AlphaToCoverageEnable = buff.alpha_to_coverage;
         m_desc.IndependentBlendEnable = buff.independent_blend;

         auto descs = buff.blend_descriptions;
         for (size_t i = 0; i < descriptors::NUM_RENDER_TARGETS; i++)
         {
            m_desc.RenderTarget[i] = descs[i].d3d_version();
         }

         for (auto i = 0; i < 4; i++)
         {
            m_blendFactor[i] = static_cast<float>(buff.blend_factor[i]);
         }
      }

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
      const D3D12_BLEND_DESC& description() const
      {
         return m_desc;
      }

      /*
       The sample mask for the blend state.
       Used by a pipeline state object.
       */
      UINT mask() const noexcept
      {
         return m_mask;
      }

      const float* blend_factor() const noexcept
      {
         return m_blendFactor;
      }

      /*
       Swaps the contents of l and r.
       */
      inline friend void swap(blender& l, blender& r) noexcept
      {
         using std::swap;
         swap(l.m_desc, r.m_desc);
         swap(l.m_mask, r.m_mask);
         swap(l.m_blendFactor, r.m_blendFactor);
      }

      /*
       Assignment Operator
       */
      inline blender& operator=(blender r) noexcept
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