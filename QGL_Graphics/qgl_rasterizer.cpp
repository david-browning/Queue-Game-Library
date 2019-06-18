#include "pch.h"
#include "include/Content/qgl_rasterizer.h"

namespace qgl::content
{
   rasterizer::rasterizer(const wchar_t* name,
                          qgl::content::content_id id) :
      m_desc(CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT)),
      content_item(name, id,
                   qgl::content::RESOURCE_TYPE_DESCRIPTION,
                   qgl::content::CONTENT_LOADER_ID_RASTERIZER)
   {
   }

   rasterizer::rasterizer(const content::buffers::RASTERIZER_BUFFER* buff,
                          const wchar_t* name,
                          qgl::content::content_id id) :
      content_item(name, id,
                   qgl::content::RESOURCE_TYPE_DESCRIPTION,
                   qgl::content::CONTENT_LOADER_ID_RASTERIZER)
   {
      m_desc.FillMode = static_cast<D3D12_FILL_MODE>(buff->FillMode);
      m_desc.CullMode = static_cast<D3D12_CULL_MODE>( buff->CullMode);
      m_desc.FrontCounterClockwise = buff->IsFrontCounterClockwise;
      m_desc.DepthBias = buff->DepthBias;
      m_desc.DepthBiasClamp = buff->DepthBiasClamp;
      m_desc.SlopeScaledDepthBias = buff->SlopeScaledDepthBias;
      m_desc.DepthClipEnable = buff->IsDepthClip;
      m_desc.MultisampleEnable = buff->IsMultisample;
      m_desc.AntialiasedLineEnable = buff->IsAntialiasedLine;
      m_desc.ForcedSampleCount = buff->ForcedSampleCount;
      m_desc.ConservativeRaster = 
         static_cast<D3D12_CONSERVATIVE_RASTERIZATION_MODE>(
            buff->ConservativeRaster);
   }

   const D3D12_RASTERIZER_DESC* rasterizer::description() const
   {
      return &m_desc;
   }
}