#include "pch.h"
#include "include\Content\Content-Buffers\qgl_sampler_buffer.h"

namespace qgl::content::buffers
{
   static constexpr float DEFAULT_SAMPLER_BORDER_COLOR[4] =
   {
      0.0f, 0.0f, 0.0f, 0.0f
   };

   SAMPLER_BUFFER::SAMPLER_BUFFER() :
      MipLODBias(0.0f),
      MaxLOD(0.0f),
      MinLOD(0.0f),
      MaxAnisotropy(1),
      Filter(D3D12_FILTER_MIN_MAG_MIP_POINT),
      ComparisonFunction(D3D12_COMPARISON_FUNC_LESS),
      Reserved1(0),
      Reserved2(0),
      Reserved3(0)
   {
      AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
      AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
      AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
      copy_elements<float>(BorderColor,
                           DEFAULT_SAMPLER_BORDER_COLOR,
                           4);
   }

   D3D12_FILTER SAMPLER_BUFFER::filter() const noexcept
   {
      return static_cast<D3D12_FILTER>(Filter);
   }

   D3D12_TEXTURE_ADDRESS_MODE SAMPLER_BUFFER::address_u() const noexcept
   {
      return static_cast<D3D12_TEXTURE_ADDRESS_MODE>(AddressU);
   }

   D3D12_TEXTURE_ADDRESS_MODE SAMPLER_BUFFER::address_v() const noexcept
   {
      return static_cast<D3D12_TEXTURE_ADDRESS_MODE>(AddressV);
   }

   D3D12_TEXTURE_ADDRESS_MODE SAMPLER_BUFFER::address_w() const noexcept
   {
      return static_cast<D3D12_TEXTURE_ADDRESS_MODE>(AddressW);
   }

   float SAMPLER_BUFFER::mip_lod_bias() const noexcept
   {
      return MipLODBias;
   }

   D3D12_COMPARISON_FUNC SAMPLER_BUFFER::comparision() const noexcept
   {
      return static_cast<D3D12_COMPARISON_FUNC>(ComparisonFunction);
   }

   const float* SAMPLER_BUFFER::border_color() const noexcept
   {
      return BorderColor;
   }

   float SAMPLER_BUFFER::min_lod() const noexcept
   {
      return MinLOD;
   }

   float SAMPLER_BUFFER::max_lod() const noexcept
   {
      return MaxLOD;
   }

   uint32_t SAMPLER_BUFFER::max_anisotropy() const noexcept
   {
      return MaxAnisotropy;
   }
}