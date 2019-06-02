#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::content::buffers
{
   struct QGL_GRAPHICS_API SAMPLER_BUFFER
   {
      public:
      SAMPLER_BUFFER();

      SAMPLER_BUFFER(const SAMPLER_BUFFER&) = default;

      SAMPLER_BUFFER(SAMPLER_BUFFER&&) = default;

      ~SAMPLER_BUFFER() noexcept = default;

      D3D12_FILTER filter() const noexcept;

      D3D12_TEXTURE_ADDRESS_MODE address_u() const noexcept;

      D3D12_TEXTURE_ADDRESS_MODE address_v() const noexcept;

      D3D12_TEXTURE_ADDRESS_MODE address_w() const noexcept;

      float mip_lod_bias() const noexcept;

      D3D12_COMPARISON_FUNC comparision() const noexcept;

      const float* border_color() const noexcept;

      float min_lod() const noexcept;

      float max_lod() const noexcept;

      uint32_t max_anisotropy() const noexcept;

      friend void swap(SAMPLER_BUFFER& l,
                       SAMPLER_BUFFER& r) noexcept
      {
         using std::swap;
         swap(l.BorderColor, r.BorderColor);
         swap(l.MipLODBias, r.MipLODBias);
         swap(l.MaxLOD, r.MaxLOD);
         swap(l.MinLOD, r.MinLOD);
         swap(l.MaxAnisotropy, r.MaxAnisotropy);
         swap(l.Filter, r.Filter);
         swap(l.AddressU, r.AddressU);
         swap(l.AddressV, r.AddressV);
         swap(l.AddressW, r.AddressW);
         swap(l.ComparisonFunction, r.ComparisonFunction);
         swap(l.Reserved1, r.Reserved1);
         swap(l.Reserved2, r.Reserved2);
         swap(l.Reserved3, r.Reserved3);
      }

      SAMPLER_BUFFER& operator=(SAMPLER_BUFFER r) noexcept
      {
         swap(*this, r);
         return *this;
      }
      
      /*
       Border color to use if D3D12_TEXTURE_ADDRESS_MODE_BORDER is specified
       for AddressU, AddressV, or AddressW. Range must be between 0.0 and 1.0 
       inclusive.
       */
      float BorderColor[4];
      
      /*
       Offset from the calculated mipmap level. For example, if the runtime 
       calculates that a texture should be sampled at mipmap level 3 and
       MipLODBias is 2, the texture will be sampled at mipmap level 5.
       */
      float MipLODBias;
      
      /*
       Upper end of the mipmap range to clamp access to, where 0 is the largest 
       and most detailed mipmap level and any level higher than that is less 
       detailed. This value must be greater than or equal to MinLOD. To have no 
       upper limit on LOD, set this member to a large value.
       */
      float MaxLOD;

      /*
       Lower end of the mipmap range to clamp access to, where 0 is the largest
       and most detailed mipmap level and any level higher than that is less 
       detailed.
       */
      float MinLOD;

      /*
       Clamping value used if D3D12_FILTER_ANISOTROPIC or 
       D3D12_FILTER_COMPARISON_ANISOTROPIC is specified in Filter. Valid values
       are between 1 and 16.
       */
      uint32_t MaxAnisotropy;

      /*
       A D3D12_FILTER-typed value that specifies the filtering method to use 
       when sampling a texture.
       */
      uint8_t Filter;
      
      /*
       A D3D12_TEXTURE_ADDRESS_MODE-typed value that specifies the method to 
       use for resolving a u texture coordinate that is outside the 0 to 1 
       range.
       */
      uint8_t AddressU;

      /*
       A D3D12_TEXTURE_ADDRESS_MODE-typed value that specifies the method to 
       use for resolving a v texture coordinate that is outside the 0 to 1 
       range.
       */
      uint8_t AddressV;

      /*
       A D3D12_TEXTURE_ADDRESS_MODE-typed value that specifies the method to 
       use for resolving a w texture coordinate that is outside the 0 to 1 
       range.
       */
      uint8_t AddressW;

      /*
       A D3D12_COMPARISON_FUNC-typed value that specifies a function that 
       compares sampled data against existing sampled data.
       */
      uint8_t ComparisonFunction;
      uint8_t Reserved1;
      uint8_t Reserved2;
      uint8_t Reserved3;
   };
}