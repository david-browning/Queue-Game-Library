#include "pch.h"
#include "include/GPU/Descriptors/qgl_dsv_descriptor_heap.h"

namespace qgl::graphics::gpu
{
   dsv_descriptor_heap::dsv_descriptor_heap(static_ptr_ref<d3d_device> dev_p,
                                            size_t numEntries, 
                                            UINT nodeMask) :
      descriptor_heap(dev_p, numEntries, nodeMask)
   {
   }
}