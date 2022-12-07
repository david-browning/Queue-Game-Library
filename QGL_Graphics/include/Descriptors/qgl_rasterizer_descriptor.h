#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::descriptors
{
#pragma pack(push, 1)
   struct rasterizer_descriptor final
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
      constexpr rasterizer_descriptor()
      {
      }

      constexpr rasterizer_descriptor(const rasterizer_descriptor&) = default;

      constexpr rasterizer_descriptor(
         rasterizer_descriptor&&) noexcept = default;

      ~rasterizer_descriptor() noexcept = default;

      friend void swap(rasterizer_descriptor& l,
         rasterizer_descriptor& r) noexcept
      {
         using std::swap;
         swap(l.depth_bias_clamp, r.depth_bias_clamp);
         swap(l.slope_scaled_depth_bias, r.slope_scaled_depth_bias);
         swap(l.depth_bias, r.depth_bias);
         swap(l.forced_sample_count, r.forced_sample_count);
         swap(l.fill_mode, r.fill_mode);
         swap(l.cull_mode, r.cull_mode);
         swap(l.conservative_raster, r.conservative_raster);
         swap(l.front_counterclockwise, r.front_counterclockwise);
         swap(l.depth_clip, r.depth_clip);
         swap(l.multisample, r.multisample);
         swap(l.antialiased_line, r.antialiased_line);
      }

      rasterizer_descriptor& operator=(rasterizer_descriptor r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      /*
       Maximum depth bias of a pixel.
       For info about depth bias, see https://tinyurl.com/d3d-depth-bias
       */
      math::rational<int32_t> depth_bias_clamp;

      /*
       Scalar on a given pixel's slope.
       For info about depth bias, see https://tinyurl.com/d3d-depth-bias
       */
      math::rational<int32_t> slope_scaled_depth_bias;

      /*
       Depth value added to a given pixel.
       For info about depth bias, see https://tinyurl.com/d3d-depth-bias
       */
      int32_t depth_bias = D3D12_DEFAULT_DEPTH_BIAS;

      /*
       The sample count that is forced while UAV rendering or rasterizing.
       Valid values are 0, 1, 2, 4, 8, and optionally 16.
       0 indicates that the sample count is not forced.
       */
      uint32_t forced_sample_count = 0;

      /*
       A D3D12_FILL_MODE-typed value that specifies the fill mode to use
       when rendering.
       */
      uint8_t fill_mode = D3D12_FILL_MODE_SOLID;

      /*
       A D3D12_CULL_MODE-typed value that specifies that triangles facing the
       specified direction are not drawn.
       */
      uint8_t cull_mode = D3D12_CULL_MODE_BACK;

      /*
       A D3D12_CONSERVATIVE_RASTERIZATION_MODE-typed value that identifies
       whether conservative rasterization is on or off.
       */
      uint8_t conservative_raster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

      /*
       Determines if a triangle is front- or back-facing.
       If this member is TRUE, a triangle will be considered front-facing if
       its vertices are counter-clockwise on the render target and considered
       back-facing if they are clockwise. If this parameter is FALSE,
       the opposite is true.
       */
      uint8_t front_counterclockwise = 0;

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
      uint8_t depth_clip = 1;

      /*
       Specifies whether to use the quadrilateral or alpha line anti-aliasing
       algorithm on multisample antialiasing (MSAA) render targets.
       Set to TRUE to use the quadrilateral line anti-aliasing algorithm and
       to FALSE to use the alpha line anti-aliasing algorithm.
       */
      uint8_t multisample = 0;

      /*
       Specifies whether to enable line antialiasing;
       only applies if doing line drawing and MultisampleEnable is FALSE.
       */
      uint8_t antialiased_line = 0;
   };
#pragma pack(pop)
}