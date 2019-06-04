#include "pch.h"
#include "include/GPU/Render/qgl_blender.h"

namespace qgl::graphics::gpu::render
{
   blender::blender() :
      m_desc(CD3DX12_BLEND_DESC(D3D12_DEFAULT)),
      m_mask(UINT_MAX)
   {
      static const float DEFAULT_BLEND[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
      memcpy(m_blendFactor, DEFAULT_BLEND, sizeof(float) * 4);
   }

   blender::blender(
      const content::buffers::BLENDER_BUFFER* buff) :
      m_desc(),
      m_mask(UINT_MAX)
   {
      m_desc.AlphaToCoverageEnable = buff->alpha_coverage();
      m_desc.IndependentBlendEnable = buff->independent_blend();

      auto descs = buff->blend_descs();
      for (size_t i = 0;
           i < content::buffers::NUM_RENDER_TARGETS;
           i++)
      {
         m_desc.RenderTarget[i] = descs[i].d3d_version();
      }

      memcpy(m_blendFactor, buff->blend_factor(), sizeof(float) * 4);
   }

   const D3D12_BLEND_DESC* blender::description() const
   {
      return &m_desc;
   }

   UINT blender::mask() const noexcept
   {
      return m_mask;
   }
   const float * blender::blend_factor() const noexcept
   {
      return nullptr;
   }
}