#pragma once
#include "include/qgl_graphics_include.h"
#include "include/qgl_graphics_device.h"
#include "include/GPU/Memory/qgl_igpu_allocator.h"
#include "include/Helpers/qgl_graphics_device_helpers.h"

namespace qgl::graphics::gpu
{
   template<class SRWTraits>
   class tex2d_allocator : public igpu_allocator
   {
      public:
      tex2d_allocator(graphics_device* dev_p,
                      size_t budget,
                      size_t alignment,
                      const D3D12_CLEAR_VALUE& clearValue) :
         m_gDev_p(dev_p),
         m_clearValue(clearValue),
         m_memStats()
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

      tex2d_allocator(const tex2d_allocator&) = delete;

      tex2d_allocator(tex2d_allocator&& r) noexcept
      {
         std::scoped_lock l{ r.m_mutex };
         m_callbacks = std::move(r.m_callbacks);
         m_allocations = std::move(r.m_allocations);
         m_memStats = std::move(r.m_memStats);
         m_gDev_p = r.m_gDev_p;
         r.m_gDev_p = nullptr;
      }

      virtual ~tex2d_allocator() noexcept = default;

      virtual gpu_alloc_handle alloc(const D3D12_RESOURCE_DESC& desc)
      {

         static const D3D12_HEAP_PROPERTIES heapProps =
            CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
         static const D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE;

         gpu_alloc_handle ret = 0;
         {
            std::scoped_lock l{ m_mutex };
            if (desc.Alignment != m_memStats.alignment)
            {
               throw std::invalid_argument{
                  "The description's alignment does not match the allocator's alignment."
               };
            }

            // Get the allocation info
            auto allocInfo = m_gDev_p->dev_3d()->GetResourceAllocationInfo(
               m_gDev_p->dev_node(), 1, &desc);

            if (m_memStats.committed + allocInfo.SizeInBytes > m_memStats.budgeted)
            {
               throw std::bad_alloc{};
            }

            // Allocate a buffer the resource
            pptr<igpu_resource> newResource_p;
            check_result(m_gDev_p->dev_3d()->CreateCommittedResource(
               &heapProps, heapFlags,
               &desc,
               D3D12_RESOURCE_STATE_DEPTH_WRITE,
               &m_clearValue,
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

         // Notify any callbacks.
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
      D3D12_CLEAR_VALUE m_clearValue;
      slim_uset<mem_budget_changed_callback, SRWTraits> m_callbacks;
      handle_map<allocation_entry, gpu_alloc_handle, SRWTraits> m_allocations;
      allocator_stats m_memStats;
      graphics_device* m_gDev_p;
   };
}