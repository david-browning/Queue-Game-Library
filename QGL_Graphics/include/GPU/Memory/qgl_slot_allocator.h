#pragma once
#include "include/qgl_graphics_include.h"
#include "include/qgl_graphics_device.h"
#include "include/GPU/Memory/qgl_igpu_allocator.h"

namespace qgl::graphics::gpu
{
   /*
    Allocates memory by placing same sized resources contiguously in a large
    heap.
    */
   class slot_allocator : public igpu_allocator
   {
      public:
      slot_allocator(graphics_device* dev_p,
                     size_t placeSize,
                     size_t numPlaces,
                     size_t alignment,
                     D3D12_HEAP_FLAGS heapFlags) :
         m_dev_p(dev_p),
         // Align the placement size.
         m_placeSize(mem::align_address(placeSize, alignment)),
         m_numPlaces(numPlaces)
      {
         gpu_mem_info info;
         check_result(helpers::app_mem(
            dev_p->adapter(), dev_p->dev_node(), &info));

         auto totalSize = m_placeSize * m_numPlaces;
         m_memStats.alignment = alignment;
         m_memStats.allocations = 0;
         m_memStats.commits = 0;
         m_memStats.budgeted = totalSize;
         m_memStats.committed = 0;
         m_memStats.total = info.available;

         // Fill the free list.
         for (size_t curOffset = 0;
              curOffset < totalSize;
              curOffset += m_placeSize)
         {
            m_freeList.push_back(curOffset);
         }

         // Allocate a heap to hold all the memory.
         CD3DX12_HEAP_DESC heapDesc(
            m_memStats.budgeted, D3D12_HEAP_TYPE_DEFAULT, 0, heapFlags);
         check_result(m_dev_p->dev_3d()->CreateHeap(
            &heapDesc, IID_PPV_ARGS(m_heap_p.put())));
      }

      slot_allocator(const slot_allocator&) = delete;

      slot_allocator(slot_allocator&& r) noexcept
      {
         // Prevent changes to r.
         std::scoped_lock l{ r.m_mutex };
         m_callbacks = std::move(r.m_callbacks);
         m_allocations = std::move(r.m_allocations);
         m_memStats = std::move(r.m_memStats);
         m_heap_p = std::move(r.m_heap_p);
         m_dev_p = r.m_dev_p;
         m_freeList = std::move(r.m_freeList);
         m_placeSize = r.m_placeSize;
         m_numPlaces = r.m_numPlaces;

         r.m_heap_p = nullptr;
         r.m_dev_p = nullptr;
      }

      virtual ~slot_allocator() noexcept
      {
         m_allocations.clear();
         m_callbacks.clear();
         m_heap_p = nullptr;
         m_dev_p = nullptr;
      }

      virtual gpu_alloc_handle alloc(const D3D12_RESOURCE_DESC& desc)
      {
         gpu_alloc_handle ret = 0;
         {
            std::scoped_lock l{ m_mutex };
            if (m_freeList.empty())
            {
               throw std::bad_alloc{};
            }

            // gpop is an atomic operation.
            auto offset = m_freeList.gpop_front();

            // Create a resource for the memory allocation.
            pptr<igpu_resource> resource;
            check_result(m_dev_p->dev_3d()->CreatePlacedResource(
               m_heap_p.get(),
               static_cast<UINT64>(offset),
               &desc,
               D3D12_RESOURCE_STATE_COMMON,
               nullptr,
               IID_PPV_ARGS(resource.put())));

            // Put the resource in our map.
            ret = m_allocations.alloc(std::move(resource));

            add_mem();
         }

         // Notify any callbacks.
         m_callbacks.lock();
         for (auto it = m_callbacks.cbegin(); it != m_callbacks.cend(); it++)
         {
            std::invoke(**it, this);
         }
         m_callbacks.unlock();
         return ret;
      }

      virtual void free(gpu_alloc_handle h)
      {
         if (m_allocations.allocated(h))
         {
            {
               std::scoped_lock l{ m_mutex };
               // Free the allocation.
               m_allocations.free(h);

               // Update the stats
               sub_mem();

               // Add the handle back to the free list.
               m_freeList.push_back(h);
            }

            // Notify any callbacks.
            m_callbacks.lock();
            for (auto it = m_callbacks.cbegin(); it != m_callbacks.cend(); it++)
            {
               std::invoke(**it, this);
            }
            m_callbacks.unlock();
         }
      }

      virtual const allocator_stats& stats() const noexcept
      {
         return m_memStats;
      }

      virtual void register_callback(mem_budget_changed_callback c)
      {
         m_callbacks.insert(c);
      }

      virtual void revoke_callback(mem_budget_changed_callback c)
      {
         m_callbacks.erase(c);
      }

      virtual igpu_resource* get(gpu_alloc_handle h)
      {
         return m_allocations.get(h).get();
      }

      virtual const igpu_resource* get(gpu_alloc_handle h) const
      {
         return m_allocations.get(h).get();
      }

      private:
      void add_mem()
      {
         m_memStats.allocations++;
         m_memStats.commits++;
         m_memStats.committed += m_placeSize;
      }

      void sub_mem()
      {
         m_memStats.allocations--;
         m_memStats.commits--;
         m_memStats.committed -= m_placeSize;
      }

      std::mutex m_mutex;

      ts_uset<mem_budget_changed_callback> m_callbacks;

      handle_map<pptr<igpu_resource>, gpu_alloc_handle> m_allocations;

      allocator_stats m_memStats;

      /*
       The heap that backs the placed resources.
       */
      pptr<igpu_heap> m_heap_p;

      /*
       Pointer to the graphics device. Use this to create resources.
       */
      graphics_device* m_dev_p;

      /*
       List of free heap offsets. Use a set instead if you want to check that
       an address was allocated before trying to free it.
       */
      ts_list<gpu_alloc_handle> m_freeList;

      size_t m_placeSize;

      size_t m_numPlaces;
   };
}