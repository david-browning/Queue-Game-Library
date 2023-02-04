#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Memory/qgl_allocator_stats.h"

namespace qgl::graphics::gpu
{
   /*
    Forward declaration of a GPU allocator.
    */
   class igpu_allocator;

   /*
    Abstract handle to an allocated GPU resource. Allocators are free to use
    this handle as they see fit.
    */
   using gpu_alloc_handle = typename uintptr_t;

   enum class mem_priorities
   {
      /*
       This resource is always committed. It must be explicitly freed.
       */
      critical,

      /*
       Allocators may replace this resource with lower or higher resolution 
       versions as the memory budget changes.
       */
      scalable,

      /*
       Allocators may commit and evict this item as needed.
       */
      streaming,
   };

   /*
    A callback that is raised when the memory budget changes. This will fire
    before and after a GPU allocation or a resource is committed or evicted.
    */
   typedef void (*mem_budget_changed_callback)(igpu_allocator*);

   class igpu_allocator
   {
      public:
      igpu_allocator()
      {

      }

      igpu_allocator(const igpu_allocator&) = default;
      
      igpu_allocator(igpu_allocator&&) noexcept = default;
      
      virtual ~igpu_allocator() noexcept = default;

      /*
       Allocates a block of memory in the GPU's virtual address space.
       */
      virtual gpu_alloc_handle alloc(const D3D12_RESOURCE_DESC& desc) = 0;

      /*
       Only free resources that were allocated by the same allocator.
       Trying to free a "gpu_alloc_handle" allocated by a different allocator
       will cause undefined behavior.
       Do not free a resource more than once. This will cause undefined
       behavior.
       */
      virtual void free(gpu_alloc_handle) = 0;

      virtual const allocator_stats& stats() const noexcept = 0;

      virtual void register_callback(mem_budget_changed_callback) = 0;

      virtual void revoke_callback(mem_budget_changed_callback) = 0;

      /*
       Looks up a resource from the resource handle.
       Returns nullptr if the handle is not valid.
       Depending on the allocation, a "freed" handle could point to a resource
       that replaced the freed one. It is the caller's responsibility to
       ensure that they only call this method with a valid handle.
       */
      virtual igpu_resource* get(gpu_alloc_handle) = 0;

      /*
       Looks up a resource from the resource handle.
       Returns nullptr if the handle is not valid.
       Depending on the allocation, a "freed" handle could point to a resource
       that replaced the freed one. It is the caller's responsibility to
       ensure that they only call this method with a valid handle.
       */
      virtual const igpu_resource* get(gpu_alloc_handle) const = 0;

      /*
       Looks up the GPU virtual address for a resource that was allocated using
       this allocator. Throws std::bad_alloc if the resource is not valid.
       Like with looking up the resource, a "freed" handle could point to a
       resource that replaced a freed one.
       */
      virtual D3D12_GPU_VIRTUAL_ADDRESS where(gpu_alloc_handle hndl)
      {
         auto r = get(hndl);
         if (!r)
         {
            throw std::bad_alloc{};
         }

         return r->GetGPUVirtualAddress();
      }

      protected:
      struct allocation_entry final
      {
         allocation_entry(pptr<igpu_resource>&& p,
                          D3D12_RESOURCE_ALLOCATION_INFO&& i) :
            resource_p(std::forward<pptr<igpu_resource>>(p)),
            info(std::forward<D3D12_RESOURCE_ALLOCATION_INFO>(i))
         {

         }

         allocation_entry(const allocation_entry&) = default;

         allocation_entry(allocation_entry&&) noexcept = default;

         ~allocation_entry() noexcept = default;

         friend void swap(allocation_entry& l, allocation_entry& r) noexcept
         {
            using std::swap;
            swap(l.info, r.info);
            swap(l.resource_p, r.resource_p);
         }

         allocation_entry& operator=(allocation_entry r) noexcept
         {
            swap(*this, r);
            return *this;
         }

         pptr<igpu_resource> resource_p;
         D3D12_RESOURCE_ALLOCATION_INFO info;
      };
   };
}