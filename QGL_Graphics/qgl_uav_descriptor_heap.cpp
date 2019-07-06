#include "pch.h"
#include "include/GPU/Descriptors/qgl_uav_descriptor_heap.h"

namespace qgl::graphics::gpu
{
   uav_descriptor_heap::uav_descriptor_heap(static_ptr_ref<d3d_device> dev_p,
                                            size_t numEntries,
                                            UINT nodeMask) :
      descriptor_heap(dev_p, numEntries, nodeMask)
   {
   }
   
   descriptor_table uav_descriptor_heap::table(
      size_t startIndex, size_t numDescriptors, 
      UINT rootIndex, UINT baseRegister, 
      D3D12_DESCRIPTOR_RANGE_FLAGS flags, 
      D3D12_SHADER_VISIBILITY visibility, 
      UINT regSpace)
   {
      descriptor_range range(D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_UAV,
                                  numDescriptors,
                                  baseRegister,
                                  flags,
                                  regSpace);

      return descriptor_table({ range },
                                   at_gpu(startIndex),
                                   rootIndex,
                                   visibility);
   }
}