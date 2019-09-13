#include "pch.h"
#include "include\Content\Content-Buffers\qgl_sampler_buffer.h"
using namespace qgl::mem;

namespace qgl::content::buffers
{
   static constexpr math::rational<int32_t> DEFAULT_SAMPLER_BORDER_COLOR[4] =
   {
      math::rational<int32_t>(),
      math::rational<int32_t>(),
      math::rational<int32_t>(),
      math::rational<int32_t>()
   };

   SAMPLER_BUFFER::SAMPLER_BUFFER() :
      MipLODBias(),
      MaxLOD(),
      MinLOD(),
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
      copy_elements<math::rational<int32_t>>(BorderColor,
                                             DEFAULT_SAMPLER_BORDER_COLOR,
                                             4);
   }
}