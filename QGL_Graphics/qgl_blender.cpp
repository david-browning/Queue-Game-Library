#include "pch.h"
#include "include\Content\qgl_blender.h"

namespace qgl::content
{
   blender::blender(const wchar_t* name,
                    qgl::content::content_id id) :
      m_desc(CD3DX12_BLEND_DESC(D3D12_DEFAULT)),
      m_mask(UINT_MAX),
      content_item(
         name, id,
         qgl::content::RESOURCE_TYPES::RESOURCE_TYPE_BLENDER,
         qgl::content::CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BLENDER_CONFIG)
   {
   }

   blender::blender(
      const content::buffers::BLENDER_BUFFER* buff,
      const wchar_t* name,
      qgl::content::content_id id) :
      m_desc(),
      m_mask(UINT_MAX),
      content_item(
         name, id,
         qgl::content::RESOURCE_TYPES::RESOURCE_TYPE_BLENDER,
         qgl::content::CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BLENDER_CONFIG)
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
   }

   const D3D12_BLEND_DESC* blender::description() const
   {
      return &m_desc;
   }

   UINT blender::mask() const noexcept
   {
      return m_mask;
   }
}