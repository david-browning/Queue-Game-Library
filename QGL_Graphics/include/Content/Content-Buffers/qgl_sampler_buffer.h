#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics::content::buffers
{
   struct QGL_GRAPHICS_API SAMPLER_BUFFER
   {
      public:
      SAMPLER_BUFFER();

      SAMPLER_BUFFER(const SAMPLER_BUFFER&) = default;

      SAMPLER_BUFFER(SAMPLER_BUFFER&&) = default;

      ~SAMPLER_BUFFER() noexcept = default;

      D3D12_FILTER filter() const;

      D3D12_TEXTURE_ADDRESS_MODE address_u() const;

      D3D12_TEXTURE_ADDRESS_MODE address_v() const;

      D3D12_TEXTURE_ADDRESS_MODE address_w() const;

      float mip_lod_bias() const;

      D3D12_COMPARISON_FUNC comparision() const;

      const float* border_color() const;

      float min_lod() const;

      float max_lod() const;

      uint32_t max_anisotropy() const;

      friend void swap(SAMPLER_BUFFER& l,
                       SAMPLER_BUFFER& r) noexcept;

      SAMPLER_BUFFER& operator=(SAMPLER_BUFFER r) noexcept;

      private:
      float m_borderColor[4];
      float m_mipLODBias;
      float m_maxLOD;
      float m_minLOD;
      uint32_t m_maxAnisotropy;

      uint8_t m_filter;
      uint8_t m_addressU;
      uint8_t m_addressV;
      uint8_t m_addressW;
      uint8_t m_comparisonFunction;
      uint8_t m_reserved1;
      uint8_t m_reserved2;
      uint8_t m_reserved3;
   };
}