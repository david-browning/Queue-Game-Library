#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::content::buffers
{
   struct QGL_GRAPHICS_API RASTERIZER_BUFFER
   {
      public:
      /*
       Constructs a default buffer.
       The fill mode is D3D12_FILL_MODE_SOLID.
       The cull mode is D3D12_CULL_MODE_BACK.
       The front counter clockwise is FALSE.
       The Depth bias is 0.
       The depth bias clamp is 0.0f.
       The slope scaled depth bias is 0.0f.
       The depth clip is enabled.
       Multisampling is disabled.
       Antialiased lines are disabled.
       Forced sample count is 0.
       Conservative rast is D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF
       */
      RASTERIZER_BUFFER();

      RASTERIZER_BUFFER(const RASTERIZER_BUFFER&) = default;

      RASTERIZER_BUFFER(RASTERIZER_BUFFER&&) = default;

      ~RASTERIZER_BUFFER() noexcept = default;

      D3D12_FILL_MODE fill_mode() const noexcept;

      D3D12_CULL_MODE cull_mode() const noexcept;

      BOOL front_counter_clockwise() const noexcept;

      INT depth_bias() const noexcept;

      FLOAT depth_bias_clamp() const noexcept;

      FLOAT slope_scaled_depth_bias() const noexcept;

      BOOL depth_clip() const noexcept;

      BOOL multisample() const noexcept;

      BOOL antialiased_lines() const noexcept;

      UINT forced_sample_counit() const noexcept;

      D3D12_CONSERVATIVE_RASTERIZATION_MODE conservative_rasterization_mode()
         const noexcept;

      friend void swap(RASTERIZER_BUFFER& l,
                       RASTERIZER_BUFFER& r) noexcept
      {
         using std::swap;
         swap(l.DepthBiasClamp, r.DepthBiasClamp);
         swap(l.SlopeScaledDepthBias, r.SlopeScaledDepthBias);
         swap(l.DepthBias, r.DepthBias);
         swap(l.ForcedSampleCount, r.ForcedSampleCount);
         swap(l.FillMode, r.FillMode);
         swap(l.CullMode, r.CullMode);
         swap(l.ConservativeRaster, r.ConservativeRaster);
         swap(l.IsFrontCounterClockwise, r.IsFrontCounterClockwise);
         swap(l.IsDepthClip, r.IsDepthClip);
         swap(l.IsMultisample, r.IsMultisample);
         swap(l.IsAntialiasedLine, r.IsAntialiasedLine);
      }

      RASTERIZER_BUFFER& operator=(RASTERIZER_BUFFER r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      /*
       Maximum depth bias of a pixel. 
       For info about depth bias, see https://tinyurl.com/d3d-depth-bias
       */
      float DepthBiasClamp;

      /*
       Scalar on a given pixel's slope.
       For info about depth bias, see https://tinyurl.com/d3d-depth-bias
       */
      float SlopeScaledDepthBias;

      /*
       Depth value added to a given pixel. 
       For info about depth bias, see https://tinyurl.com/d3d-depth-bias
       */
      int32_t DepthBias;
      
      /*
       The sample count that is forced while UAV rendering or rasterizing. 
       Valid values are 0, 1, 2, 4, 8, and optionally 16. 
       0 indicates that the sample count is not forced.
       */
      uint32_t ForcedSampleCount;

      /*
       A D3D12_FILL_MODE-typed value that specifies the fill mode to use 
       when rendering.
       */
      uint8_t FillMode;

      /*
       A D3D12_CULL_MODE-typed value that specifies that triangles facing the 
       specified direction are not drawn.
       */
      uint8_t CullMode;

      /*
       A D3D12_CONSERVATIVE_RASTERIZATION_MODE-typed value that identifies 
       whether conservative rasterization is on or off.
       */
      uint8_t ConservativeRaster;

      /*
       Determines if a triangle is front- or back-facing. 
       If this member is TRUE, a triangle will be considered front-facing if 
       its vertices are counter-clockwise on the render target and considered 
       back-facing if they are clockwise. If this parameter is FALSE, 
       the opposite is true.
       */
      uint8_t IsFrontCounterClockwise;

      /*
       Specifies whether to enable clipping based on distance.
       The hardware always performs x and y clipping of rasterized coordinates. 
       When DepthClipEnable is set to the default–TRUE, the hardware also clips 
       the z value.
       When you set DepthClipEnable to FALSE, the hardware skips the z clipping 
       (that is, the last step in the preceding algorithm). However, the 
       hardware still performs the "0 < w" clipping. When z clipping is 
       disabled, improper depth ordering at the pixel level might result. 
       However, when z clipping is disabled, stencil shadow implementations 
       are simplified. In other words, you can avoid complex special-case 
       handling for geometry that goes beyond the back clipping plane.
       */
      uint8_t IsDepthClip;
      
      /*
       Specifies whether to use the quadrilateral or alpha line anti-aliasing 
       algorithm on multisample antialiasing (MSAA) render targets. 
       Set to TRUE to use the quadrilateral line anti-aliasing algorithm and 
       to FALSE to use the alpha line anti-aliasing algorithm.
       */
      uint8_t IsMultisample;

      /*
       Specifies whether to enable line antialiasing; 
       only applies if doing line drawing and MultisampleEnable is FALSE. 
       */
      uint8_t IsAntialiasedLine;
   };
}