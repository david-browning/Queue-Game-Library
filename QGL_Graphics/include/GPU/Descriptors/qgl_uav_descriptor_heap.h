#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Descriptors/qgl_descriptor_table.h"
#include "include/GPU/Descriptors/qgl_descriptor_heap.h"

namespace qgl::graphics::gpu
{
   class uav_descriptor_heap :
      public descriptor_heap<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
      D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE>
   {
      public:
      uav_descriptor_heap(i3d_device* dev_p,
                          size_t numEntries,
                          UINT nodeMask = 0) :
         descriptor_heap(dev_p, numEntries, nodeMask)
      {

      }


      uav_descriptor_heap(const uav_descriptor_heap&) = default;

      uav_descriptor_heap(uav_descriptor_heap&&) = default;

      virtual ~uav_descriptor_heap() noexcept = default;

      /*
       startIndex: The index where to start in this descriptor table.

       numDescriptors: The number of descriptors for the table to include. It must be at least 1.

       rootIndex: index in the root signature where the table will be bound.

       baseRegister: The base shader register in the range. For example, for shader-resource
       views (SRVs), 3 maps to ": register(t3);" in HLSL.

       flags: https://docs.microsoft.com/en-us/windows/desktop/api/d3d12/ne-d3d12-d3d12_descriptor_range_flags

       regSpace: The register space. Can typically be 0, but allows multiple descriptor arrays of
       unknown size to not appear to overlap. For example, for SRVs, by extending the example in the
       BaseShaderRegister member description, 5 maps to ": register(t3,space5);" in HLSL.

       numDescriptors must be at least 1.
       */
      virtual descriptor_table table(
         size_t startIndex,
         size_t numDescriptors,
         UINT rootIndex,
         UINT baseRegister,
         D3D12_DESCRIPTOR_RANGE_FLAGS flags,
         D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL,
         UINT regSpace = 0)
      {
         descriptor_range range(
            D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_UAV,
            numDescriptors,
            baseRegister,
            flags,
            regSpace);

         return descriptor_table(
            { range },
            at_gpu(startIndex),
            rootIndex,
            visibility);
      }
   };
}