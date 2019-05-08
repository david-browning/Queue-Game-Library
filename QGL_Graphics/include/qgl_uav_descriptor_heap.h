#pragma once
#include "qgl_graphics_include.h"
#include "qgl_descriptor_heap.h"

namespace qgl::graphics
{
   //using uav_descriptor_heap = descriptor_heap<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV>;
   class LIB_EXPORT uav_descriptor_heap : public descriptor_heap<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV>
   {
      public:
      uav_descriptor_heap(winrt::com_ptr<d3d_device>& dev_p,
                          size_t numEntries,
                          D3D12_DESCRIPTOR_HEAP_FLAGS heapFlag,
                          UINT nodeMask = 0);

      uav_descriptor_heap(const uav_descriptor_heap& r) = delete;

      uav_descriptor_heap(uav_descriptor_heap&& r);

      virtual ~uav_descriptor_heap();

     /*
      startIndex: The index where to start in this descriptor table.

      numDescriptors: The number of descriptors for the table to include. It must be at least 1.

      rootIndex: Index in the root signature where the table will be bound.

      baseRegister: The base shader register in the range. For example, for shader-resource
      views (SRVs), 3 maps to ": register(t3);" in HLSL.

      flags: https://docs.microsoft.com/en-us/windows/desktop/api/d3d12/ne-d3d12-d3d12_descriptor_range_flags

      regSpace: The register space. Can typically be 0, but allows multiple descriptor arrays of
      unknown size to not appear to overlap. For example, for SRVs, by extending the example in the
      BaseShaderRegister member description, 5 maps to ": register(t3,space5);" in HLSL.

      DBC: numDescriptors must be at least 1.
      */
      virtual _DBC_ low::descriptor_table table(size_t startIndex,
                                                size_t numDescriptors,
                                                UINT rootIndex,
                                                UINT baseRegister,
                                                D3D12_DESCRIPTOR_RANGE_FLAGS flags,
                                                D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL,
                                                UINT regSpace = 0);
   };
}