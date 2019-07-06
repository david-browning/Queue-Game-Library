#include "pch.h"
#include "include/GPU/Render/qgl_rasterizer.h"

namespace qgl::graphics::gpu::render
{
   rasterizer::rasterizer(const content::buffers::RASTERIZER_BUFFER* buff)
   {
      m_desc.FillMode = static_cast<D3D12_FILL_MODE>(buff->FillMode);
      m_desc.CullMode = static_cast<D3D12_CULL_MODE>(buff->CullMode);
      m_desc.FrontCounterClockwise = buff->IsFrontCounterClockwise;
      m_desc.DepthBias = buff->DepthBias;
      m_desc.DepthBiasClamp = static_cast<float>(buff->DepthBiasClamp);
      m_desc.SlopeScaledDepthBias =
         static_cast<float>(buff->SlopeScaledDepthBias);
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