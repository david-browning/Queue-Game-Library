#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Descriptors/qgl_descriptor_heap.h"

namespace qgl::graphics::gpu
{
   class QGL_GRAPHICS_API dsv_descriptor_heap : 
      public descriptor_heap<D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
                             D3D12_DESCRIPTOR_HEAP_FLAG_NONE>
   {
      public:
      dsv_descriptor_heap(d3d_device* dev_p,
                          size_t numEntries,
                          UINT nodeMask = 0);

      dsv_descriptor_heap(const dsv_descriptor_heap&) = default;

      dsv_descriptor_heap(dsv_descriptor_heap&&) = default;

      virtual ~dsv_descriptor_heap() = default;
   };
}