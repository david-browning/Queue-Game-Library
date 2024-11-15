#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Memory/qgl_igpu_allocator.h"

namespace qgl::graphics::gpu
{
   template<D3D12_RESOURCE_STATES State, D3D12_HEAP_TYPE HeapType, class SRWTraits>
   class committed_allocator : public igpu_allocator
   {
      public:
      committed_allocator(graphics_device* dev_p,
                          size_t budget,
                          size_t alignment) :
         m_gDev_p(dev_p)
      {
         gpu_mem_info info;
         check_result(helpers::app_mem(
            dev_p->adapter(), dev_p->dev_node(), &info));

         m_memStats.alignment = alignment;
         m_memStats.allocations = 0;
         m_memStats.commits = 0;
         m_memStats.committed = 0;
         m_memStats.budgeted = budget;
         m_memStats.total = info.available;
      }

      committed_allocator(const committed_allocator&) = delete;

      committed_allocator(committed_allocator&& r)
      {
         std::scoped_lock l{ r.m_mutex };
         m_callbacks = std::move(r.m_callbacks);
         m_allocations = std::move(r.m_allocations);
         m_memStats = std::move(r.m_memStats);
         m_gDev_p = r.m_gDev_p;
         r.m_gDev_p = nullptr;
      }

      virtual ~committed_allocator() noexcept = default;

      virtual gpu_alloc_handle alloc(const D3D12_RESOURCE_DESC& desc)
      {
         if (desc.Alignment > 0 && desc.Alignment != m_memStats.alignment)
         {
            throw std::invalid_argument{
               "The description's alignment does not match the allocator's alignment."
            };
         }

         gpu_alloc_handle ret = 0;
         
         // Do the allocation
         {
            std::scoped_lock l{ m_mutex };

            auto allocInfo = m_gDev_p->dev_3d()->GetResourceAllocationInfo(
              m_gDev_p->dev_node(), 1, &desc);

            if (m_memStats.committed + allocInfo.SizeInBytes >
               m_memStats.budgeted)
            {
               // Out of budget
               throw std::bad_alloc{};
            }

            // Allocate a buffer the resource
            pptr<igpu_resource> newResource_p;
            static const auto heapProps = CD3DX12_HEAP_PROPERTIES(HeapType);
            static const auto heapFlags = D3D12_HEAP_FLAG_NONE;
            check_result(m_gDev_p->dev_3d()->CreateCommittedResource(
               &heapProps, 
               heapFlags,
               &desc,
               State,
               nullptr,
               IID_PPV_ARGS(newResource_p.put())));

            allocation_entry allocation{
              std::move(newResource_p),
              std::move(allocInfo),
            };

            add_mem(allocation);
            ret = m_allocations.alloc(std::move(allocation));
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
         {
            std::scoped_lock l{ m_mutex };
            const auto& entry = m_allocations.get(h);
            sub_mem(entry);
            m_allocations.free(h);
         }

         m_callbacks.lock();
         for (auto it = m_callbacks.cbegin(); it != m_callbacks.cend(); it++)
         {
            std::invoke(**it, this);
         }
         m_callbacks.unlock();
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
         return m_allocations.get(h).resource_p.get();
      }

      virtual const igpu_resource* get(gpu_alloc_handle h) const
      {
         return m_allocations.get(h).resource_p.get();
      }

      private:
      void add_mem(const allocation_entry& e)
      {
         m_memStats.commits++;
         m_memStats.allocations++;
         m_memStats.committed += e.info.SizeInBytes;
      }

      void sub_mem(const allocation_entry& e)
      {
         m_memStats.commits--;
         m_memStats.allocations--;
         m_memStats.committed -= e.info.SizeInBytes;
      }

      std::mutex m_mutex;
      slim_uset<mem_budget_changed_callback, SRWTraits> m_callbacks;
      handle_map<allocation_entry, gpu_alloc_handle, SRWTraits> m_allocations;
      allocator_stats m_memStats;
      graphics_device* m_gDev_p;
   };

   template<class SRWTraits = srw_traits>
   using copy_src_allocator = typename committed_allocator<
      D3D12_RESOURCE_STATE_COMMON, D3D12_HEAP_TYPE_DEFAULT, SRWTraits>;
   
   template<class SRWTraits = srw_traits>
   using copy_dst_allocator = typename committed_allocator<
      D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_HEAP_TYPE_UPLOAD, SRWTraits>;
   
   template<class SRWTraits = srw_traits>
   using vert_allocator = typename committed_allocator<
      D3D12_RESOURCE_STATE_COPY_DEST, D3D12_HEAP_TYPE_DEFAULT, SRWTraits>;

   template<class SRWTraits = srw_traits>
   using idx_allocator = typename vert_allocator<SRWTraits>;
}