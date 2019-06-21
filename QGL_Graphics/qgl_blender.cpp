#include "pch.h"
#include "include/Content/qgl_blender.h"

namespace qgl::content
{
   blender::blender(const wchar_t* name,
                    id_t id) :
      m_desc(CD3DX12_BLEND_DESC(D3D12_DEFAULT)),
      m_mask(UINT_MAX),
      content_item(name,
                   id,
                   RESOURCE_TYPE_DESCRIPTION,
                   CONTENT_LOADER_ID_BLENDER)
   {
      static const float DEFAULT_BLEND[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
      memcpy(m_blendFactor, DEFAULT_BLEND, sizeof(float) * 4);
   }

   blender::blender(const buffers::BLENDER_BUFFER* buff,
                    const wchar_t* name,
                    id_t id) :
      m_desc(),
      m_mask(UINT_MAX),
      content_item(name,
                   id,
                   RESOURCE_TYPE_DESCRIPTION,
                   CONTENT_LOADER_ID_BLENDER)
   {
      m_desc.AlphaToCoverageEnable = buff->IsAlphaToCoverage;
      m_desc.IndependentBlendEnable = buff->IsIndependentBlend;

      auto descs = buff->BlendDescriptions;
      for (size_t i = 0;
           i < buffers::NUM_RENDER_TARGETS;
           i++)
      {
         m_desc.RenderTarget[i] = descs[i].d3d_version();
      }

      for (auto i = 0; i < 4; i++)
      {
         m_blendFactor[i] = buff->BlendFactor[i];
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