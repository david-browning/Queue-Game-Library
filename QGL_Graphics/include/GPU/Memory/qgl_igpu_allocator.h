#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics::gpu
{
   /*
    Abstract handle to an allocated GPU resource. Allocators are free to use
    this handle as they see fit.
    */
   using gpu_alloc_handle = typename uintptr_t;

   class igpu_allocator
   {
      public:

      virtual ~igpu_allocator() = default;

      /*
       Allocates a block of memory in the GPU's virtual address space.
       After allocating memory, the allocator must synchronize with the GPU by
       calling "synchronize".
       Throws std::bad_alloc if there is no memory available.
       */
      virtual gpu_alloc_handle alloc(const D3D12_RESOURCE_DESC& description,
                                     D3D12_RESOURCE_STATES initialState) = 0;

      /*
       Only free resources that were allocated by the same allocator.
       Trying to free a "gpu_alloc_handle" allocated by a different allocator
       will cause undefined behavior.
       Do not free a resource more than once. This will cause undefined
       behavior.
       */
      virtual void free(gpu_alloc_handle) = 0;

      /*
       Returns the total amount of memory that was allocated for this
       allocator.
       */
      virtual size_t size() const noexcept = 0;

      /*
       Returns the amount of memory that is in use. Just because there is
       enough left-over memory to allocate another object, doesn't mean
       allocation will succeed. There may be wasted space due to alignment.
       */
      virtual size_t used() const noexcept = 0;

      /*
       Looks up a resource from the resource handle.
       Returns nullptr if the handle is not valid.
       Depending on the allocation, a "freed" handle could point to a resource
       that replaced the freed one. It is the caller's responsibility to
       ensure that they only call this method with a valid handle.
       */
      virtual igpu_resource* resource(gpu_alloc_handle) = 0;

      /*
       Looks up a resource from the resource handle.
       Returns nullptr if the handle is not valid.
       Depending on the allocation, a "freed" handle could point to a resource
       that replaced the freed one. It is the caller's responsibility to
       ensure that they only call this method with a valid handle.
       */
      virtual const igpu_resource* resource(gpu_alloc_handle) const = 0;

      /*
       Returns a pointer to the heap that backs the allocated memory.
       */
      virtual igpu_heap* backing() = 0;

      /*
       Returns a const pointer to the heap that backs the allocated memory.
       */
      virtual const igpu_heap* backing() const = 0;


      /*
       Looks up the GPU virtual address for a resource that was allocated using
       this allocator. Throws std::bad_alloc if the resource is not valid.
       Like with looking up the resource, a "freed" handle could point to a
       resource that replaced a freed one.
       */
      virtual D3D12_GPU_VIRTUAL_ADDRESS where(gpu_alloc_handle hndl)
      {
         auto r = resource(hndl);
         if (!r)
         {
            throw std::bad_alloc{};
         }

         return r->GetGPUVirtualAddress();
      }
   };

   using gpu_allocator_ptr = typename std::shared_ptr<igpu_allocator>;
}