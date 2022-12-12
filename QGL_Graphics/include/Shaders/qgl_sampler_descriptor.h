#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Descriptors/qgl_vector_descriptor.h"

namespace qgl::graphics::shaders
{
#pragma pack(push, 1)
   struct sampler_descriptor final
   {
      public:
      constexpr sampler_descriptor()
      {
      }

      constexpr sampler_descriptor(const sampler_descriptor&) = default;

      constexpr sampler_descriptor(sampler_descriptor&&) noexcept = default;

      ~sampler_descriptor() noexcept = default;

      friend void swap(sampler_descriptor& l,
                       sampler_descriptor& r) noexcept
      {
         using std::swap;
         swap(l.border, r.border);
         swap(l.mip_lod_bias, r.mip_lod_bias);
         swap(l.lod_max, r.lod_max);
         swap(l.lod_min, r.lod_min);
         swap(l.anisotropy_max, r.anisotropy_max);
         swap(l.filter, r.filter);
         swap(l.addressu, r.addressu);
         swap(l.addressv, r.addressv);
         swap(l.addressw, r.addressw);
         swap(l.comparison_func, r.comparison_func);
         swap(l.reserved1, r.reserved1);
         swap(l.reserved2, r.reserved2);
         swap(l.reserved3, r.reserved3);
      }

      sampler_descriptor& operator=(sampler_descriptor r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      /*
       Border color to use if D3D12_TEXTURE_ADDRESS_MODE_BORDER is specified
       for AddressU, AddressV, or AddressW. Range must be between 0.0 and 1.0
       inclusive.
       */
      descriptors::vector_descriptor border;

      /*
       Offset from the calculated mipmap level. For example, if the runtime
       calculates that a texture should be sampled at mipmap level 3 and
       mip_lod_bias is 2, the texture will be sampled at mipmap level 5.
       */
      math::rational<int32_t> mip_lod_bias;

      /*
       Upper end of the mipmap range to clamp access to, where 0 is the largest
       and most detailed mipmap level and any level higher than that is less
       detailed. This value must be greater than or equal to lod_min. To have no
       upper limit on LOD, set this member to a large value.
       */
      math::rational<int32_t> lod_max;

      /*
       Lower end of the mipmap range to clamp access to, where 0 is the largest
       and most detailed mipmap level and any level higher than that is less
       detailed.
       */
      math::rational<int32_t> lod_min;

      /*
       Clamping value used if D3D12_FILTER_ANISOTROPIC or
       D3D12_FILTER_COMPARISON_ANISOTROPIC is specified in Filter. Valid values
       are between 1 and 16.
       */
      uint32_t anisotropy_max = 1;

      /*
       A D3D12_FILTER-typed value that specifies the filtering method to use
       when sampling a texture.
       */
      uint8_t filter = D3D12_FILTER_MIN_MAG_MIP_POINT;

      /*
       A D3D12_TEXTURE_ADDRESS_MODE-typed value that specifies the method to
       use for resolving a u texture coordinate that is outside the 0 to 1
       range.
       */
      uint8_t addressu = D3D12_TEXTURE_ADDRESS_MODE_BORDER;

      /*
       A D3D12_TEXTURE_ADDRESS_MODE-typed value that specifies the method to
       use for resolving a v texture coordinate that is outside the 0 to 1
       range.
       */
      uint8_t addressv = D3D12_TEXTURE_ADDRESS_MODE_BORDER;

      /*
       A D3D12_TEXTURE_ADDRESS_MODE-typed value that specifies the method to
       use for resolving a w texture coordinate that is outside the 0 to 1
       range.
       */
      uint8_t addressw = D3D12_TEXTURE_ADDRESS_MODE_BORDER;

      /*
       A D3D12_COMPARISON_FUNC-typed value that specifies a function that
       compares sampled data against existing sampled data.
       */
      uint8_t comparison_func = D3D12_COMPARISON_FUNC_LESS;
      uint8_t reserved1 = 0;
      uint8_t reserved2 = 0;
      uint8_t reserved3 = 0;

      /*
       Returns a D3D12_SAMPLER_DESC from this.
       */
      explicit operator D3D12_SAMPLER_DESC() const noexcept
      {
         D3D12_SAMPLER_DESC desc = {};
         desc.AddressU = static_cast<D3D12_TEXTURE_ADDRESS_MODE>(addressu);
         desc.AddressV = static_cast<D3D12_TEXTURE_ADDRESS_MODE>(addressv);
         desc.AddressW = static_cast<D3D12_TEXTURE_ADDRESS_MODE>(addressw);
         
         desc.BorderColor[0] = static_cast<float>(border[0]);
         desc.BorderColor[1] = static_cast<float>(border[1]);
         desc.BorderColor[2] = static_cast<float>(border[2]);
         desc.BorderColor[3] = static_cast<float>(border[3]);
         
         desc.ComparisonFunc = static_cast<D3D12_COMPARISON_FUNC>(
            comparison_func);
         desc.Filter = static_cast<D3D12_FILTER>(filter);
         desc.MaxAnisotropy = anisotropy_max;
         desc.MaxLOD = static_cast<float>(lod_max);
         desc.MinLOD = static_cast<float>(lod_min);
         desc.MipLODBias = static_cast<float>(mip_lod_bias);
         return desc;
      }
   };
#pragma pack(pop)
}