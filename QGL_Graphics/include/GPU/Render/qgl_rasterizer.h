#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/Content-Descriptors/qgl_rasterizer_descriptor.h"

namespace qgl::graphics::gpu
{
   class rasterizer
   {
      public:
      /*
       Constructs a rasterizer from the buffer.
       */
      rasterizer(const descriptors::rasterizer_descriptor& buff)
      {
         m_desc.FillMode = static_cast<D3D12_FILL_MODE>(buff.fill_mode);
         m_desc.CullMode = static_cast<D3D12_CULL_MODE>(buff.cull_mode);
         m_desc.FrontCounterClockwise = buff.front_counterclockwise;
         m_desc.DepthBias = buff.depth_bias;
         m_desc.DepthBiasClamp = static_cast<float>(buff.depth_bias_clamp);
         m_desc.SlopeScaledDepthBias =
            static_cast<float>(buff.slope_scaled_depth_bias);
         m_desc.DepthClipEnable = buff.depth_clip;
         m_desc.MultisampleEnable = buff.multisample;
         m_desc.AntialiasedLineEnable = buff.antialiased_line;
         m_desc.ForcedSampleCount = buff.forced_sample_count;
         m_desc.ConservativeRaster =
            static_cast<D3D12_CONSERVATIVE_RASTERIZATION_MODE>(
               buff.conservative_raster);
      }

      /*
       Copy constructor.
       */
      rasterizer(const rasterizer&) = default;

      /*
       Move constructor.
       */
      rasterizer(rasterizer&&) = default;

      /*
       Destructor.
       */
      virtual ~rasterizer() noexcept = default;

      /*
       Returns a pointer to the D3D rasterizer description.
       Used by the pipeline state object.
       */
      const D3D12_RASTERIZER_DESC& description() const
      {
         return m_desc;
      }

      private:
      D3D12_RASTERIZER_DESC m_desc;
   };
}