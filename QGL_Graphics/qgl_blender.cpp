#include "pch.h"
#include "include/GPU/Render/qgl_blender.h"

namespace qgl::graphics::gpu::render
{
   blender::blender(const content::buffers::BLENDER_BUFFER* buff) :
      m_desc(),
      m_mask(UINT_MAX)
   {
      m_desc.AlphaToCoverageEnable = buff->IsAlphaToCoverage;
      m_desc.IndependentBlendEnable = buff->IsIndependentBlend;

      auto descs = buff->BlendDescriptions;
      for (size_t i = 0;
           i < content::buffers::NUM_RENDER_TARGETS;
           i++)
      {
         m_desc.RenderTarget[i] = descs[i].d3d_version();
      }

      for (auto i = 0; i < 4; i++)
      {
         m_blendFactor[i] = static_cast<float>(buff->BlendFactor[i]);
      }
   }

   const D3D12_BLEND_DESC* blender::description() const
   {
      return &m_desc;
   }

   UINT blender::mask() const noexcept
   {
      return m_mask;
   }
   const float* blender::blend_factor() const noexcept
   {
      return nullptr;
   }
}