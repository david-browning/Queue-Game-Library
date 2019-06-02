#include "pch.h"
#include "include/GPU/Descriptors/qgl_descriptor_range.h"

namespace qgl::graphics::gpu
{
   descriptor_range::descriptor_range(D3D12_DESCRIPTOR_RANGE_TYPE rangeType,
                                      size_t numDescriptors,
                                      UINT baseRegister, 
                                      D3D12_DESCRIPTOR_RANGE_FLAGS flags, 
                                      UINT regSpace)
   {
      m_range.Init(rangeType,
                   static_cast<UINT>(numDescriptors),
                   baseRegister,
                   regSpace,
                   flags);
   }

   const D3D12_DESCRIPTOR_RANGE1* descriptor_range::range() const noexcept
   {
      return &m_range;
   }
}