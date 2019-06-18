#include "pch.h"
#include "include\Content\Content-Buffers\qgl_rasterizer_buffer.h"

namespace qgl::content::buffers
{
   RASTERIZER_BUFFER::RASTERIZER_BUFFER() :
      DepthBiasClamp(),
      SlopeScaledDepthBias(),
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
}