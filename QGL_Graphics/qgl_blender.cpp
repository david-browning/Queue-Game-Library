#include "pch.h"
#include "include/qgl_blender.h"

qgl::graphics::blender::blender() :
   m_desc(CD3DX12_BLEND_DESC(D3D12_DEFAULT)),
   m_mask(UINT_MAX)
{
}

qgl::graphics::blender::blender(const BLENDER_BUFFER& buff) :
   m_desc(),
   m_mask(UINT_MAX)
{
   m_desc.AlphaToCoverageEnable = buff.alpha_coverage();
   m_desc.IndependentBlendEnable = buff.independent_blend();

   auto descs = buff.blend_descs();
   for (size_t i = 0; i < buff.description_count(); i++)
   {
      m_desc.RenderTarget[i] = descs[i].d3d_version();
   }
}

qgl::graphics::blender::blender(const blender& r) :
   m_desc(r.m_desc),
   m_mask(r.m_mask)
{
}

qgl::graphics::blender::blender(blender&& r) :
   m_desc(std::move(r.m_desc)),
   m_mask(r.m_mask)
{
}

qgl::graphics::blender::~blender()
{
}
