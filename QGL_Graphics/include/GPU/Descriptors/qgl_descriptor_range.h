#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics::gpu
{
   /*
    baseRegister: The base shader register in the range.
    For example, for shader - resource views(SRVs), 3 maps to ": register(t3);" 
    in HLSL.
    regSpace : The register space.Can typically be 0, but allows multiple 
     descriptor arrays of unknown size to not appear to overlap. 
     For example, for SRVs, by extending the example in the BaseShaderRegister 
     member description, 5 maps to ": register(t3,space5);" in HLSL.
    flags : https://tinyurl.com/d3d12-descriptorrangeflgs
   */
   class QGL_GRAPHICS_API descriptor_range
   {
      public:
      descriptor_range(D3D12_DESCRIPTOR_RANGE_TYPE rangeType,
                       size_t numDescriptors,
                       UINT baseRegister,
                       D3D12_DESCRIPTOR_RANGE_FLAGS flags,
                       UINT regSpace = 0);

      descriptor_range(const descriptor_range&) = default;

      descriptor_range(descriptor_range&&) = default;

      virtual ~descriptor_range() noexcept = default;

      const D3D12_DESCRIPTOR_RANGE1* range() const noexcept;

      private:
      CD3DX12_DESCRIPTOR_RANGE1 m_range;
   };
}