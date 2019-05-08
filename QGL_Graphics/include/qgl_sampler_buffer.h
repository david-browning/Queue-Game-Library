#pragma once
#include "qgl_graphics_include.h"

namespace qgl::graphics
{
   struct LIB_EXPORT SAMPLER_BUFFER
   {
      public:
      SAMPLER_BUFFER();

      SAMPLER_BUFFER(const SAMPLER_BUFFER& r);

      SAMPLER_BUFFER(SAMPLER_BUFFER&& r);

      ~SAMPLER_BUFFER() noexcept;

      D3D12_FILTER filter() const
      {
         return static_cast<D3D12_FILTER>(m_filter);
      }

      D3D12_TEXTURE_ADDRESS_MODE address_u() const
      {
         return static_cast<D3D12_TEXTURE_ADDRESS_MODE>(m_addressU);
      }

      D3D12_TEXTURE_ADDRESS_MODE address_v() const
      {
         return static_cast<D3D12_TEXTURE_ADDRESS_MODE>(m_addressV);
      }

      D3D12_TEXTURE_ADDRESS_MODE address_w() const
      {
         return static_cast<D3D12_TEXTURE_ADDRESS_MODE>(m_addressW);
      }

      float mip_lod_bias() const
      {
         return m_mipLODBias;
      }

      D3D12_COMPARISON_FUNC comparision() const
      {
         return static_cast<D3D12_COMPARISON_FUNC>(m_comparisonFunction);
      }

      const float* border_color() const
      {
         return m_borderColor;
      }

      float min_lod() const
      {
         return m_minLOD;
      }

      float max_lod() const
      {
         return m_maxLOD;
      }

      uint32_t max_anisotropy() const
      {
         return m_maxAnisotropy;
      }

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