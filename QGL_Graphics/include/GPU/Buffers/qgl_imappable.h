#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics::gpu
{
   /*
    A mappable object is one that can be accessed by the CPU and GPU.
    Note that unlike D3D11, the resource does not need to be unmapped for use
    by the GPU.
    */
   template<class T>
   class imappable
   {
      public:
      imappable() {}

      imappable(const imappable&) = delete;

      imappable(imappable&&) = default;

      virtual ~imappable() = default;

      /*
       Maps a GPU resource to the CPU pointer. if the resource is already 
       mapped, this should do nothing.
       */
      virtual void map() = 0;

      /*
       Unmaps the CPU pointer. If the resource is not mapped, this should do
       nothing.
       */
      virtual void unmap() = 0;

      /*
       Returns a pointer to the mapped CPU memory. Returns nullptr if not 
       mapped.
       */
      virtual const T* mapping() const = 0;

      /*
       Returns a pointer to the mapped CPU memory. Returns nullptr if not
       mapped.
       */
      virtual T* mapping() = 0;

      /*
       Returns true if the resource is mapped so the CPU can access it.
       */
      virtual bool mapped() const noexcept = 0;
   };
}