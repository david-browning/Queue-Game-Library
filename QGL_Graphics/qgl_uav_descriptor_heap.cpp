#include "pch.h"
#include "include/qgl_uav_descriptor_heap.h"

qgl::graphics::uav_descriptor_heap::uav_descriptor_heap(winrt::com_ptr<d3d_device>& dev_p,
                                                        size_t numEntries,
                                                        D3D12_DESCRIPTOR_HEAP_FLAGS heapFlag,
                                                        UINT nodeMask) :
   descriptor_heap(dev_p, numEntries, heapFlag, nodeMask)
{
}

qgl::graphics::uav_descriptor_heap::uav_descriptor_heap(uav_descriptor_heap&& r) :
   descriptor_heap(std::move(r))
{
}

qgl::graphics::uav_descriptor_heap::~uav_descriptor_heap()
{
}

qgl::graphics::low::descriptor_table qgl::graphics::uav_descriptor_heap::table(
   size_t startIndex,
   size_t numDescriptors,
   UINT rootIndex,
   UINT baseRegister,
   D3D12_DESCRIPTOR_RANGE_FLAGS flags,
   D3D12_SHADER_VISIBILITY visibility,
   UINT regSpace)
{
   low::descriptor_range range(D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_UAV,
                               numDescriptors,
                               baseRegister,
                               flags,
                               regSpace);

   return low::descriptor_table({ range },
                                at_gpu(startIndex),
                                rootIndex,
                                visibility);
}
