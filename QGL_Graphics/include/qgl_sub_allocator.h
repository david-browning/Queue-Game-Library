#pragma once
#include "qgl_graphics_include.h"
#include "qgl_memory.h"
#include "qgl_imem_allocator.h"

namespace qgl::graphics::low
{
   template<size_t UAlignment>
   class sub_allocator : public imemory_allocator<UAlignment>
   {
      static_assert(!(UAlignment == 0 || (UAlignment & (UAlignment - 1))),
                    "UAlignment must be a power of 2.");

      public:
      sub_allocator(size_t heapSize,
                    winrt::com_ptr<d3d_device>& dev_p);

      sub_allocator(const sub_allocator& r) = delete;

      sub_allocator(sub_allocator&& r) = delete;

      virtual ~sub_allocator();

      virtual D3D12_GPU_VIRTUAL_ADDRESS allocate(size_t buffSize)
      {
         //If the list of allocations is empty, then just add a new one.
      }

      virtual void deallocate(const D3D12_GPU_VIRTUAL_ADDRESS& addr);

      template<typename T>
      T* pointer(const D3D12_GPU_VIRTUAL_ADDRESS& addr);

      private:

      struct ALLOC_BLOCK
      {
         D3D12_GPU_VIRTUAL_ADDRESS GPUAddr;
         uintptr_t CPUAddr;
         size_t Size;
         bool Allocated;
      };

      std::list<ALLOC_BLOCK> m_allocations;
      winrt::com_ptr<d3d_resource> m_bigHeap;
      winrt::com_ptr<d3d_device> m_dev_p;
   };
}