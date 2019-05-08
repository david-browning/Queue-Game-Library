#pragma once
#include "qgl_graphics_include.h"

namespace qgl::graphics::low
{
   /*
    baseRegister: The base shader register in the range. For example, for shader-resource 
     views (SRVs), 3 maps to ": register(t3);" in HLSL.
    regSpace: The register space. Can typically be 0, but allows multiple descriptor arrays of
     unknown size to not appear to overlap. For example, for SRVs, by extending the example in the
     BaseShaderRegister member description, 5 maps to ": register(t3,space5);" in HLSL.
    flags: https://docs.microsoft.com/en-us/windows/desktop/api/d3d12/ne-d3d12-d3d12_descriptor_range_flags
    */
   class LIB_EXPORT descriptor_range
   {
      public:
      descriptor_range(D3D12_DESCRIPTOR_RANGE_TYPE rangeType, 
                       size_t numDescriptors,
                       UINT baseRegister,
                       D3D12_DESCRIPTOR_RANGE_FLAGS flags,
                       UINT regSpace = 0)
      {
         m_range.Init(rangeType, static_cast<UINT>(numDescriptors), baseRegister, regSpace, flags);
      }

      descriptor_range(const descriptor_range& r) :
         m_range(r.m_range)
      {
      }

      descriptor_range(descriptor_range&& r) :
         m_range(std::move(r.m_range))
      {
      }

      virtual ~descriptor_range() noexcept
      {

      }

      inline const D3D12_DESCRIPTOR_RANGE1& range() const
      {
         return m_range;
      }

      private:
      CD3DX12_DESCRIPTOR_RANGE1 m_range;
   };
}