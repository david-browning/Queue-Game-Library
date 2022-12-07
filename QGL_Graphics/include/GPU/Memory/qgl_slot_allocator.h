#pragma once
#include "include/qgl_graphics_include.h"
#include "include/qgl_graphics_device.h"
#include "include/GPU/Memory/qgl_igpu_allocator.h"

namespace qgl::graphics::gpu
{
   /*
    Allocates memory by placing same sized resources contiguously in a large
    buffer.
    TODO: If we can find a way to move d3d_resources without invalidating their
    pointers, then add support for resizing the backing heap.
    */
   template<size_t Alignment, D3D12_HEAP_FLAGS HeapFlags>
   class slot_allocator : public igpu_allocator
   {
      public:
      /*
       The "placeSize" is aligned so this may allocate a larger heap than
       "placeSize * initialNumPlaces".
       This does not own the graphics device pointer. Do not allow it to be 
       freed or go out of scope before destroying this.
       */
      slot_allocator(size_t placeSize, 
                     size_t initialNumPlaces,
                     graphics_device* dev_p) :
         m_dev_p(dev_p),
         m_numPlaces(initialNumPlaces)
      {
         // Align the placement size.
         m_placeSize = mem::align_address(placeSize, Alignment);

         // Fill the free list.
         for (size_t curOffset = 0; curOffset < size(); curOffset += m_placeSize)
         {
            m_freeList.push_back(curOffset);
         }

         // Allocate a heap to hold all the memory.
         CD3DX12_HEAP_DESC heapDesc(size(), D3D12_HEAP_TYPE_DEFAULT, 0,
            HeapFlags);
         winrt::check_hresult(m_dev_p->dev_3d()->CreateHeap(
            &heapDesc, IID_PPV_ARGS(m_heap_p.put())));
      }

      slot_allocator(const slot_allocator& r) = delete;

      /*
       This simply moves the allocation class to a new object.
       The memory this manages is not moved to a new address space.
       */
      slot_allocator(slot_allocator&& x) noexcept :
         m_heap_p(std::move(x.m_heap_p)),
         m_resources(std::move(x.m_resources)),
         m_dev_p(std::move(x.m_dev_p)),
         m_freeList(std::move(x.m_freeList)),
         m_placeSize(std::move(x.m_placeSize)),
         m_numPlaces(std::move(x.m_numPlaces)),
         igpu_allocator(std::move(x))
      {
         x.m_heap_p = nullptr;
         x.m_dev_p = nullptr;
      }

      /*
       Once the destructor is called, all memory this manages is released.
       Be sure the GPU is in a state where the memory can be freed.
       */
      virtual ~slot_allocator() noexcept
      {
         m_heap_p = nullptr;
         m_dev_p = nullptr;
      }

      gpu_alloc_handle alloc(const D3D12_RESOURCE_DESC& description,
                             D3D12_RESOURCE_STATES initialState) override
      {
         // Do not allow multiple threads to allocate or free at the same time.
         std::lock_guard allocationLock{ m_allocationMutex };

         if (m_freeList.empty())
         {
            throw std::bad_alloc{};
         }

         auto offset = m_freeList.front();
         m_freeList.pop_front();

         // Create a resource for the memory allocation.
         winrt::com_ptr<igpu_resource> resource;
         winrt::check_hresult(m_dev_p->dev_3d()->CreatePlacedResource(
            m_heap_p.get(),
            static_cast<UINT64>(offset),
            &description,
            initialState,
            nullptr,
            IID_PPV_ARGS(resource.put())));

         // Put the resource in our map.
         m_resources.insert_or_assign(
            offset,
            std::make_pair(initialState, std::move(resource)));

         return offset;
      }

      virtual void free(gpu_alloc_handle hndl)
      {
         // Do not allow multiple threads to allocate or free at the same time.
         std::lock_guard allocationLock{ m_allocationMutex };

         // Remove the resource pointer from the internal map. This calls the
         // destructor on the resource.
         m_resources.erase(hndl);

         // Add the offset to the free list.
         m_freeList.push_back(hndl);
      }

      virtual size_t size() const noexcept
      {
         return m_placeSize * m_numPlaces;
      }

      virtual size_t used() const noexcept
      {
         return size() - m_freeList.size() * m_placeSize;
      }

      virtual igpu_heap* backing()
      {
         return m_heap_p.get();
      }

      virtual const igpu_heap* backing() const
      {
         return m_heap_p.get();
      }

      virtual igpu_resource* resource(gpu_alloc_handle hndl)
      {
         if (m_resources.count(hndl) > 0)
         {
            return m_resources.at(hndl).second.get();
         }

         return nullptr;
      }

      virtual const igpu_resource* resource(gpu_alloc_handle hndl) const
      {
         if (m_resources.count(hndl) > 0)
         {
            return m_resources.at(hndl).second.get();
         }

         return nullptr;
      }

      private:
      using resource_pair = typename std::pair<
         D3D12_RESOURCE_STATES, winrt::com_ptr<igpu_resource>>;

      /*
       The heap that backs the placed resources.
       */
      winrt::com_ptr<igpu_heap> m_heap_p;

      /*
       The resources managed by this allocator. Map the offset to the resource
       so we can look up the pointer quickly.
       */
      std::unordered_map<gpu_alloc_handle, resource_pair> m_resources;

      /*
       Pointer to the graphics device. Use this to create resources.
       */
      graphics_device* m_dev_p;

      /*
       Used to block multiple threads from allocating simultaneously.
       */
      std::mutex m_allocationMutex;

      /*
       List of free heap offsets. Use a set instead if you want to check that
       an address was allocated before trying to free it.
       */
      std::list<gpu_alloc_handle> m_freeList;

      /*
       Size of resources that will be "placed" in the heap.
       */
      size_t m_placeSize;

      /*
       Number of "places" that can hold a resource.
       */
      size_t m_numPlaces;
   };
}