#pragma once
#include "qgl_graphics_include.h"
#include "qgl_descriptor_heap.h"

namespace qgl::graphics
{
   //using dsv_descriptor_heap = descriptor_heap<D3D12_DESCRIPTOR_HEAP_TYPE_DSV>;
   class LIB_EXPORT dsv_descriptor_heap : public descriptor_heap<D3D12_DESCRIPTOR_HEAP_TYPE_DSV>
   {
      public:
      dsv_descriptor_heap(winrt::com_ptr<d3d_device>& dev_p,
                          size_t numEntries,
                          D3D12_DESCRIPTOR_HEAP_FLAGS heapFlag,
                          UINT nodeMask = 0);

      dsv_descriptor_heap(const dsv_descriptor_heap& r) = delete;

      dsv_descriptor_heap(dsv_descriptor_heap&& r);

      virtual ~dsv_descriptor_heap();
   };
}