#include "pch.h"
#include "include/Content/Content-Buffers/qgl_blender_desc_buffer.h"

namespace qgl::graphics::content::buffers
{
   static const D3D12_RENDER_TARGET_BLEND_DESC
      DEFAULT_RENDER_TARGET_BLEND_DESC =
   {
       FALSE,FALSE,
       D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
       D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
       D3D12_LOGIC_OP_NOOP,
       D3D12_COLOR_WRITE_ENABLE_ALL,
   };

   BLEND_DESC::BLEND_DESC()
   {
      Reserved1 = 0;
      Reserved2 = 0;

      BlendEnable = FALSE;
      LogicOp = FALSE;
      SrcBlend = D3D12_BLEND_ONE;
      DestBlend = D3D12_BLEND_ZERO;
      BlendOp = D3D12_BLEND_OP_ADD;

      SrcBlendAlpha = D3D12_BLEND_ONE;
      DestBlendAlpha = D3D12_BLEND_ZERO;
      BlendOpAlpha = D3D12_BLEND_OP_ADD;

      LogicOp = D3D12_LOGIC_OP_NOOP;
      RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
   }

   D3D12_RENDER_TARGET_BLEND_DESC BLEND_DESC::d3d_version() const noexcept
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

   BLEND_DESC& BLEND_DESC::operator=(BLEND_DESC r) noexcept
   {
      swap(*this, r);
      return *this;
   }

   void swap(BLEND_DESC& first,
             BLEND_DESC& second) noexcept
   {
      using std::swap;
      swap(first.BlendEnable, second.BlendEnable);
      swap(first.LogicOpEnable, second.LogicOpEnable);
      swap(first.SrcBlend, second.SrcBlend);
      swap(first.DestBlend, second.DestBlend);
      swap(first.BlendOp, second.BlendOp);
      swap(first.SrcBlendAlpha, second.SrcBlendAlpha);
      swap(first.DestBlendAlpha, second.DestBlendAlpha);
      swap(first.BlendOpAlpha, second.BlendOpAlpha);
      swap(first.LogicOp, second.LogicOp);
      swap(first.RenderTargetWriteMask, second.RenderTargetWriteMask);
      swap(first.Reserved1, second.Reserved1);
      swap(first.Reserved2, second.Reserved2);
   }

   bool operator==(const BLEND_DESC& r, const BLEND_DESC& l) noexcept
   {
      return r.BlendEnable == l.BlendEnable &&
         r.LogicOpEnable == l.LogicOpEnable &&
         r.SrcBlend == l.SrcBlend &&
         r.DestBlend == l.DestBlend &&
         r.BlendOp == l.BlendOp &&
         r.SrcBlendAlpha == l.SrcBlendAlpha &&
         r.DestBlendAlpha == l.DestBlendAlpha &&
         r.BlendOpAlpha == l.BlendOpAlpha &&
         r.LogicOp == l.LogicOp &&
         r.RenderTargetWriteMask == l.RenderTargetWriteMask &&
         r.Reserved1 == l.Reserved1 &&
         r.Reserved2 == l.Reserved2;
   }
}