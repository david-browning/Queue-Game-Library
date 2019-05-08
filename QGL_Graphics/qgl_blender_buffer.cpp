#include "pch.h"
#include "include/qgl_blender_buffer.h"
using namespace qgl::graphics;

static const D3D12_RENDER_TARGET_BLEND_DESC DEFAULT_RENDER_TARGET_BLEND_DESC =
{
    FALSE,FALSE,
    D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
    D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
    D3D12_LOGIC_OP_NOOP,
    D3D12_COLOR_WRITE_ENABLE_ALL,
};

qgl::graphics::BLENDER_BUFFER::BLENDER_BUFFER() :
   m_isAlphaToCoverage(false),
   m_isIndependentBlend(false)
{
   for (size_t i = 0; i < NUM_RENDER_TARGETS; i++)
   {
      m_blend_descs[i] = DEFAULT_RENDER_TARGET_BLEND_DESC;
   }
}

qgl::graphics::BLENDER_BUFFER::BLENDER_BUFFER(const BLENDER_BUFFER& r) :
   m_isAlphaToCoverage(r.m_isAlphaToCoverage),
   m_isIndependentBlend(m_isIndependentBlend)
{
   MemoryCopy<BLEND_DESC>(m_blend_descs,
                          r.blend_descs(),
                          NUM_RENDER_TARGETS);
}

qgl::graphics::BLENDER_BUFFER::BLENDER_BUFFER(BLENDER_BUFFER&& r) :
   m_isAlphaToCoverage(r.m_isAlphaToCoverage),
   m_isIndependentBlend(m_isIndependentBlend)
{
   MemoryCopy<BLEND_DESC>(m_blend_descs,
                          r.blend_descs(),
                          NUM_RENDER_TARGETS);
}

qgl::graphics::BLEND_DESC::BLEND_DESC()
{

}

qgl::graphics::BLEND_DESC::BLEND_DESC(const BLEND_DESC& r) :
   BlendEnable(r.BlendEnable),
   BlendOp(r.BlendOp),
   BlendOpAlpha(r.BlendOpAlpha),
   DestBlend(r.DestBlend),
   DestBlendAlpha(r.DestBlendAlpha),
   LogicOp(r.LogicOp),
   LogicOpEnable(r.LogicOpEnable),
   RenderTargetWriteMask(r.RenderTargetWriteMask),
   SrcBlend(r.SrcBlend),
   SrcBlendAlpha(r.SrcBlendAlpha)
{
}

qgl::graphics::BLEND_DESC::BLEND_DESC(BLEND_DESC&& r) :
   BlendEnable(r.BlendEnable),
   BlendOp(r.BlendOp),
   BlendOpAlpha(r.BlendOpAlpha),
   DestBlend(r.DestBlend),
   DestBlendAlpha(r.DestBlendAlpha),
   LogicOp(r.LogicOp),
   LogicOpEnable(r.LogicOpEnable),
   RenderTargetWriteMask(r.RenderTargetWriteMask),
   SrcBlend(r.SrcBlend),
   SrcBlendAlpha(r.SrcBlendAlpha)
{
}

BLEND_DESC& qgl::graphics::BLEND_DESC::operator=(const BLEND_DESC& r)
{
   if (&r != this)
   {
      this->BlendEnable             = r.BlendEnable;
      this->BlendOp                 = r.BlendOp;
      this->BlendOpAlpha            = r.BlendOpAlpha;
      this->DestBlend               = r.DestBlend;
      this->DestBlendAlpha          = r.DestBlendAlpha;
      this->LogicOp                 = r.LogicOp;
      this->LogicOpEnable           = r.LogicOpEnable;
      this->RenderTargetWriteMask   = r.RenderTargetWriteMask;
      this->SrcBlend                = r.SrcBlend;
      this->SrcBlendAlpha           = r.SrcBlendAlpha;
   }

   return *this;
}

BLEND_DESC& qgl::graphics::BLEND_DESC::operator=(const D3D12_RENDER_TARGET_BLEND_DESC& r)
{
   this->BlendEnable             = static_cast<uint8_t>(r.BlendEnable);
   this->BlendOp                 = static_cast<uint8_t>(r.BlendOp);
   this->BlendOpAlpha            = static_cast<uint8_t>(r.BlendOpAlpha);
   this->DestBlend               = static_cast<uint8_t>(r.DestBlend);
   this->DestBlendAlpha          = static_cast<uint8_t>(r.DestBlendAlpha);
   this->LogicOp                 = static_cast<uint8_t>(r.LogicOp);
   this->LogicOpEnable           = static_cast<uint8_t>(r.LogicOpEnable);
   this->RenderTargetWriteMask   = static_cast<uint8_t>(r.RenderTargetWriteMask);
   this->SrcBlend                = static_cast<uint8_t>(r.SrcBlend);
   this->SrcBlendAlpha           = static_cast<uint8_t>(r.SrcBlendAlpha);

   return *this;
}

D3D12_RENDER_TARGET_BLEND_DESC qgl::graphics::BLEND_DESC::d3d_version() const
{
   D3D12_RENDER_TARGET_BLEND_DESC ret;
   ret.BlendEnable = BlendEnable;
   ret.BlendOp = static_cast<D3D12_BLEND_OP>(BlendOp);
   ret.BlendOpAlpha = static_cast<D3D12_BLEND_OP>(BlendOpAlpha);
   ret.DestBlend = static_cast<D3D12_BLEND>(DestBlend);
   ret.DestBlendAlpha = static_cast<D3D12_BLEND>(DestBlendAlpha);
   ret.LogicOp = static_cast<D3D12_LOGIC_OP>(LogicOp);
   ret.LogicOpEnable = LogicOpEnable;
   ret.RenderTargetWriteMask = RenderTargetWriteMask;
   ret.SrcBlend = static_cast<D3D12_BLEND>(SrcBlend);
   ret.SrcBlendAlpha = static_cast<D3D12_BLEND>(SrcBlendAlpha);
   return ret;
}
