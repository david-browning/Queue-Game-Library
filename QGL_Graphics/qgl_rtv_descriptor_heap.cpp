#include "pch.h"
#include "include/GPU/Descriptors/qgl_rtv_descriptor_heap.h"

namespace qgl::graphics::gpu
{
   rtv_descriptor_heap::rtv_descriptor_heap(d3d_device* dev_p, 
                                            size_t numEntries, 
                                            UINT nodeMask) :
      descriptor_heap(dev_p, numEntries, nodeMask)
   {
   }
}