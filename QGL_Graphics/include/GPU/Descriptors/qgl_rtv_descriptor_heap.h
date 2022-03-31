#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Descriptors/qgl_descriptor_heap.h"

namespace qgl::graphics::gpu
{
   class rtv_descriptor_heap : public descriptor_heap<
      D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
      D3D12_DESCRIPTOR_HEAP_FLAG_NONE>
   {
      public:
      rtv_descriptor_heap(i3d_device* dev_p,
                          size_t numEntries,
                          UINT nodeMask = 0) :
         descriptor_heap(dev_p, numEntries, nodeMask)
      {
      }

      rtv_descriptor_heap(const rtv_descriptor_heap&) = default;

      rtv_descriptor_heap(rtv_descriptor_heap&&) = default;

      virtual ~rtv_descriptor_heap() noexcept = default;
   };

}