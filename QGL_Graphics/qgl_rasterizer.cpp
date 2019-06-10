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
      m_desc.FillMode = buff->fill_mode();
      m_desc.CullMode = buff->cull_mode();
      m_desc.FrontCounterClockwise = buff->front_counter_clockwise();
      m_desc.DepthBias = buff->depth_bias();
      m_desc.DepthBiasClamp = buff->depth_bias_clamp();
      m_desc.SlopeScaledDepthBias = buff->slope_scaled_depth_bias();
      m_desc.DepthClipEnable = buff->depth_clip();
      m_desc.MultisampleEnable = buff->multisample();
      m_desc.AntialiasedLineEnable = buff->antialiased_lines();
      m_desc.ForcedSampleCount = buff->forced_sample_counit();
      m_desc.ConservativeRaster = buff->conservative_rasterization_mode();
	}

   const D3D12_RASTERIZER_DESC* rasterizer::description() const
   {
      return &m_desc;
   }
}