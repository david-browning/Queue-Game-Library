#include "pch.h"
#include "include\Content\Content-Buffers\qgl_rasterizer_buffer.h"

namespace qgl::graphics::content::buffers
{
   RASTERIZER_BUFFER::RASTERIZER_BUFFER() :
      DepthBiasClamp(D3D12_DEFAULT_DEPTH_BIAS_CLAMP),
      SlopeScaledDepthBias(D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS),
      DepthBias(D3D12_DEFAULT_DEPTH_BIAS),
      ForcedSampleCount(0),
      FillMode(D3D12_FILL_MODE_SOLID),
      CullMode(D3D12_CULL_MODE_BACK),
      ConservativeRaster(D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF),
      IsFrontCounterClockwise(false),
      IsDepthClip(true),
      IsMultisample(false),
      IsAntialiasedLine(false)
   {

   }

   D3D12_FILL_MODE RASTERIZER_BUFFER::fill_mode() const noexcept
	{
		return static_cast<D3D12_FILL_MODE>(FillMode);
	}

   D3D12_CULL_MODE RASTERIZER_BUFFER::cull_mode() const noexcept
   {
      return static_cast<D3D12_CULL_MODE>(CullMode);
   }

   BOOL RASTERIZER_BUFFER::front_counter_clockwise() const noexcept
   {
      return IsFrontCounterClockwise;
   }

   INT RASTERIZER_BUFFER::depth_bias() const noexcept
   {
      return DepthBias;
   }

   FLOAT RASTERIZER_BUFFER::depth_bias_clamp() const noexcept
   {
      return FLOAT();
   }

   FLOAT RASTERIZER_BUFFER::slope_scaled_depth_bias() const noexcept
   {
      return SlopeScaledDepthBias;
   }

   BOOL RASTERIZER_BUFFER::depth_clip() const noexcept
   {
      return IsDepthClip;
   }

   BOOL RASTERIZER_BUFFER::multisample() const noexcept
   {
      return IsMultisample;
   }

   BOOL RASTERIZER_BUFFER::antialiased_lines() const noexcept
   {
      return IsAntialiasedLine;
   }

   UINT RASTERIZER_BUFFER::forced_sample_counit() const noexcept
   {
      return ForcedSampleCount;
   }

   D3D12_CONSERVATIVE_RASTERIZATION_MODE RASTERIZER_BUFFER::
      conservative_rasterization_mode() const noexcept
   {
      return static_cast<D3D12_CONSERVATIVE_RASTERIZATION_MODE>(
         ConservativeRaster);
   }
}