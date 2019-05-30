#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics::content::buffers
{
   struct QGL_GRAPHICS_API BLEND_DESC
   {
      public:
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
      BLEND_DESC();

      /*
       Copy constructor.
       */
      BLEND_DESC(const BLEND_DESC&) = default;

      /*
       Move constructor.
       */
      BLEND_DESC(BLEND_DESC&&) = default;

      /*
       Destructor. Do not mark as virtual.
       */
      ~BLEND_DESC() noexcept = default;

      /*
       Returns a D3D12 interpretation of the blend description.
       */
      D3D12_RENDER_TARGET_BLEND_DESC d3d_version() const noexcept;

      /*
       Swaps the contents of first with second.
       */
      friend void swap(BLEND_DESC& first, BLEND_DESC& second) noexcept;

      /*
       Assignment Operator.
       */
      BLEND_DESC& operator=(BLEND_DESC r) noexcept;

      /*
       Equality operator.
       */
      friend bool operator==(const BLEND_DESC& r,
                             const BLEND_DESC& l) noexcept;

      uint32_t Reserved1;
      uint16_t Reserved2;

      /*
       Specifies whether to enable (or disable) blending. 
       Set to TRUE to enable blending.
       */
      uint8_t BlendEnable;

      /*
       Specifies whether to enable (or disable) a logical operation. Set to 
       TRUE to enable a logical operation.
       */
      uint8_t LogicOpEnable;

      /*
       A D3D12_BLEND-typed value that specifies the operation to perform on 
       the RGB value that the pixel shader outputs. The BlendOp member defines
       how to combine the SrcBlend and DestBlend operations.
       */
      uint8_t SrcBlend;
      
      /*
       A D3D12_BLEND-typed value that specifies the operation to perform on the
       current RGB value in the render target. The BlendOp member defines how 
       to combine the SrcBlend and DestBlend operations.
       */
      uint8_t DestBlend;
      
      /*
       A D3D12_BLEND_OP-typed value that defines how to combine the SrcBlend 
       and DestBlend operations.
       */
      uint8_t BlendOp;
      
      /*
       A D3D12_BLEND-typed value that specifies the operation to perform on 
       the alpha value that the pixel shader outputs. Blend options that end
       in _COLOR are not allowed. The BlendOpAlpha member defines how to 
       combine the SrcBlendAlpha and DestBlendAlpha operations.
       */
      uint8_t SrcBlendAlpha;

      /*
       A D3D12_BLEND-typed value that specifies the operation to perform on 
       the current alpha value in the render target. Blend options that end 
       in _COLOR are not allowed. The BlendOpAlpha member defines how to 
       combine the SrcBlendAlpha and DestBlendAlpha operations.
       */
      uint8_t DestBlendAlpha;

      /*
       A D3D12_BLEND_OP-typed value that defines how to combine the 
       SrcBlendAlpha and DestBlendAlpha operations.
       */
      uint8_t BlendOpAlpha;

      /*
       A D3D12_LOGIC_OP-typed value that specifies the logical operation to 
       configure for the render target.
       */
      uint8_t LogicOp;

      /*
       A combination of D3D12_COLOR_WRITE_ENABLE-typed values that are 
       combined by using a bitwise OR operation. The resulting value specifies 
       a write mask.
       */
      uint8_t RenderTargetWriteMask;
   };
}