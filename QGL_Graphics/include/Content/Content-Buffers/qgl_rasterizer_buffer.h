#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics::content::buffers
{
   struct QGL_GRAPHICS_API RASTERIZER_BUFFER
   {
      public:
      RASTERIZER_BUFFER();

      RASTERIZER_BUFFER(const RASTERIZER_BUFFER&) = default;

      RASTERIZER_BUFFER(RASTERIZER_BUFFER&&) = default;

      ~RASTERIZER_BUFFER() noexcept = default;

      D3D12_FILL_MODE fill_mode() const;

      D3D12_CULL_MODE cull_mode() const;

      BOOL front_counter_clockwise() const;

      INT depth_bias() const;

      FLOAT depth_bias_clamp() const;

      FLOAT slope_scaled_depth_bias() const;

      BOOL depth_clip() const;

      BOOL multisample() const;

      BOOL antialiased_lines() const;

      UINT forced_sample_counit() const;

      D3D12_CONSERVATIVE_RASTERIZATION_MODE conservative_rasterization_mode() const;

      friend void swap(RASTERIZER_BUFFER& l,
                       RASTERIZER_BUFFER& r) noexcept;

      RASTERIZER_BUFFER& operator=(RASTERIZER_BUFFER r) noexcept;

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