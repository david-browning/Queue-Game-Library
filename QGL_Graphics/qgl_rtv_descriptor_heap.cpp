#include "pch.h"
#include "include/qgl_rtv_descriptor_heap.h"

qgl::graphics::rtv_descriptor_heap::rtv_descriptor_heap(winrt::com_ptr<d3d_device>& dev_p,
                                                        size_t numEntries,
                                                        D3D12_DESCRIPTOR_HEAP_FLAGS heapFlag,
                                                        UINT nodeMask) :
   descriptor_heap(dev_p, numEntries, heapFlag, nodeMask)
{
}

qgl::graphics::rtv_descriptor_heap::rtv_descriptor_heap(rtv_descriptor_heap&& r) :
   descriptor_heap(std::move(r))
{
}

qgl::graphics::rtv_descriptor_heap::~rtv_descriptor_heap()
{
}
