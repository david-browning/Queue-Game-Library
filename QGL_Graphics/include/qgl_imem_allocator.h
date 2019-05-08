#pragma once
#include "qgl_graphics_include.h"
#include "qgl_memory.h"

namespace qgl::graphics::low
{
   /*
    Base GPU memory allocator. Derived classes must implement allocate() and
    deallocate(). Implementations can take different approaches. Some can allocate
    committed resources or 1 massive heap and use a ring buffer to place memory within
    the heap.

    Memory allocators are not guaranteed to be thread-safe.
    */
   template<size_t UAlignment>
   class imemory_allocator
   {
      static_assert(!(UAlignment == 0 || (UAlignment & (UAlignment - 1))),
                    "UAlignment must be a power of 2.");

      public:
      /*
       Constructor
       */
      imemory_allocator()
      {

      }

      /*
       Do not allow copying the allocator. Each allocator
       should manage a separate area of memory. None should manage the same
       area.
       */
      imemory_allocator(const imemory_allocator& r) = delete;

      /*
       There shouldn't be any need to move a memory allocator. If I do need to
       move a memory allocator, it won't actually move the memory to a new location. It
       might just move the allocation mechanism.
       */
      imemory_allocator(imemory_allocator&& r) = delete;

      virtual ~imemory_allocator()
      {

      }

      /*
       Aligns the buffer size, allocates memory on the GPU and returns the virtual address
       where the memory is.
       */
      virtual D3D12_GPU_VIRTUAL_ADDRESS allocate(size_t buffSize) = 0;

      /*
       Deallocate the virtual memory that was allocated for the given GPU address.
       */
      virtual void deallocate(const D3D12_GPU_VIRTUAL_ADDRESS& addr) = 0;
   };
}