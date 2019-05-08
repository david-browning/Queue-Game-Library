#pragma once
#include "qgl_graphics_include.h"
#include "qgl_descriptor_heap.h"

namespace qgl::graphics
{
   class LIB_EXPORT rtv_descriptor_heap : public descriptor_heap<D3D12_DESCRIPTOR_HEAP_TYPE_RTV>
   {
      public:
      rtv_descriptor_heap(winrt::com_ptr<d3d_device>& dev_p,
                          size_t numEntries,
                          D3D12_DESCRIPTOR_HEAP_FLAGS heapFlag,
                          UINT nodeMask = 0);

      rtv_descriptor_heap(const rtv_descriptor_heap& r) = delete;

      rtv_descriptor_heap(rtv_descriptor_heap&& r);

      virtual ~rtv_descriptor_heap();
   };

}