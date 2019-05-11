#pragma once
#include "qgl_graphics_include.h"

namespace qgl::graphics
{
   struct LIB_EXPORT RASTERIZER_BUFFER
   {
      public:
      RASTERIZER_BUFFER();

      RASTERIZER_BUFFER(const RASTERIZER_BUFFER& r);

      RASTERIZER_BUFFER(RASTERIZER_BUFFER&& r);

      inline D3D12_FILL_MODE fill_mode() const
      {
         return static_cast<D3D12_FILL_MODE>(m_fill_mode);
      }

      inline D3D12_CULL_MODE cull_mode() const
      {
         return static_cast<D3D12_CULL_MODE>(m_cull_mode);
      }

      inline BOOL front_counter_clockwise() const
      {
         return m_isFrontCounterClockwise;
      }

      inline INT depth_bias() const
      {
         return m_depthBias;
      }

      inline FLOAT depth_bias_clamp() const
      {
         return m_depthBiasClamp;
      }

      inline FLOAT slope_scaled_depth_bias() const
      {
         return m_slopeScaledDepthBias;
      }

      inline BOOL depth_clip() const
      {
         return m_isDepthClip;
      }

      inline BOOL multisample() const
      {
         return m_isMultisample;
      }

      inline BOOL antialiased_lines() const
      {
         return m_isAntialiasedLine;
      }

      inline UINT forced_sample_counit() const
      {
         return m_forcedSampleCount;
      }

      inline D3D12_CONSERVATIVE_RASTERIZATION_MODE conservative_rasterization_mode() const
      {
         return static_cast<D3D12_CONSERVATIVE_RASTERIZATION_MODE>(m_conservativeRaster);
      }

      private:
      float m_depthBiasClamp;
      float m_slopeScaledDepthBias;
      int32_t m_depthBias;
      uint32_t m_forcedSampleCount;
      uint8_t m_fill_mode;
      uint8_t m_cull_mode;
      uint8_t m_conservativeRaster;
      uint8_t m_isFrontCounterClockwise;
      uint8_t m_isDepthClip;
      uint8_t m_isMultisample;
      uint8_t m_isAntialiasedLine;
   };
}