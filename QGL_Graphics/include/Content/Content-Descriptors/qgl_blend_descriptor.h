#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics::descriptors
{
#pragma pack(push, 1)
   struct blend_descriptor final
   {
      /*
       Constructs a default blend description.
       Blending is disabled.
       Logic operation is disabled.

       The source blend is D3D12_BLEND_ONE
       The destination blend is D3D12_BLEND_ZERO
       The blend op is D3D12_BLEND_OP_ADD

       The source blend alpha is D3D12_BLEND_ONE
       The destination blend alpha is D3D12_BLEND_ZERO
       The alpha blend operation is D3D12_BLEND_OP_ADD

       The logic operation is D3D12_LOGIC_OP_NOOP
       The render target write mask is D3D12_COLOR_WRITE_ENABLE_ALL
       */
      constexpr blend_descriptor()
      {
      }

      /*
       Returns a D3D12 interpretation of the blend description.
       */
      D3D12_RENDER_TARGET_BLEND_DESC d3d_version() const noexcept
      {
         D3D12_RENDER_TARGET_BLEND_DESC ret;
         ret.BlendEnable = blend;
         ret.LogicOpEnable = enable_logic_op;
         ret.SrcBlend = static_cast<D3D12_BLEND>(blend_src);
         ret.DestBlend = static_cast<D3D12_BLEND>(blend_dest);
         ret.BlendOp = static_cast<D3D12_BLEND_OP>(blend_op);
         ret.SrcBlendAlpha = static_cast<D3D12_BLEND>(blend_alpha_src);
         ret.DestBlendAlpha = static_cast<D3D12_BLEND>(blend_alpha_dest);
         ret.BlendOpAlpha = static_cast<D3D12_BLEND_OP>(blend_alpha_op);
         ret.LogicOp = static_cast<D3D12_LOGIC_OP>(op_logic);
         ret.RenderTargetWriteMask = write_mask;
         return ret;
      }

      friend void swap(blend_descriptor& l, blend_descriptor& r) noexcept
      {
         using std::swap;
         swap(l.blend, r.blend);
         swap(l.enable_logic_op, r.enable_logic_op);
         swap(l.blend_src, r.blend_src);
         swap(l.blend_dest, r.blend_dest);
         swap(l.blend_op, r.blend_op);
         swap(l.blend_alpha_src, r.blend_alpha_src);
         swap(l.blend_alpha_dest, r.blend_alpha_dest);
         swap(l.blend_alpha_op, r.blend_alpha_op);
         swap(l.op_logic, r.op_logic);
         swap(l.write_mask, r.write_mask);
      }

      blend_descriptor& operator=(blend_descriptor r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      /*
       Specifies whether to enable (or disable) blending.
       Set to TRUE to enable blending.
       */
      uint8_t blend = FALSE;

      /*
       Specifies whether to enable (or disable) a logical operation. Set to
       TRUE to enable a logical operation.
       */
      uint8_t enable_logic_op = FALSE;

      /*
       A D3D12_BLEND-typed value that specifies the operation to perform on
       the RGB value that the pixel shader outputs. The blend_op member defines
       how to combine the blend_src and blend_dest operations.
       */
      uint8_t blend_src = D3D12_BLEND_ONE;

      /*
       A D3D12_BLEND-typed value that specifies the operation to perform on the
       current RGB value in the render target. The blend_op member defines how
       to combine the blend_src and blend_dest operations.
       */
      uint8_t blend_dest = D3D12_BLEND_ZERO;

      /*
       A D3D12_BLEND_OP-typed value that defines how to combine the blend_src
       and blend_dest operations.
       */
      uint8_t blend_op = D3D12_BLEND_OP_ADD;

      /*
       A D3D12_BLEND-typed value that specifies the operation to perform on
       the alpha value that the pixel shader outputs. Blend options that end
       in _COLOR are not allowed. The blend_alpha_op member defines how to
       combine the blend_alpha_src and blend_alpha_dest operations.
       */
      uint8_t blend_alpha_src = D3D12_BLEND_ONE;

      /*
       A D3D12_BLEND-typed value that specifies the operation to perform on
       the current alpha value in the render target. Blend options that end
       in _COLOR are not allowed. The blend_alpha_op member defines how to
       combine the blend_alpha_src and blend_alpha_dest operations.
       */
      uint8_t blend_alpha_dest = D3D12_BLEND_ZERO;

      /*
       A D3D12_BLEND_OP-typed value that defines how to combine the
       blend_alpha_src and blend_alpha_dest operations.
       */
      uint8_t blend_alpha_op = D3D12_BLEND_OP_ADD;

      /*
       A D3D12_LOGIC_OP-typed value that specifies the logical operation to
       configure for the render target.
       */
      uint8_t op_logic = D3D12_LOGIC_OP_NOOP;

      /*
       A combination of D3D12_COLOR_WRITE_ENABLE-typed values that are
       combined by using a bitwise OR operation. The resulting value specifies
       a write mask.
       */
      uint8_t write_mask = D3D12_COLOR_WRITE_ENABLE_ALL;
   };
#pragma pack(pop)
}