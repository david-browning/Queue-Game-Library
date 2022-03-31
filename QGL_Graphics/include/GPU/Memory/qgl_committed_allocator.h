#pragma once
#include "include/qgl_graphics_include.h"
#include "include/qgl_graphics_device.h"
#include "include/GPU/Memory/qgl_igpu_allocator.h"

namespace qgl::graphics::gpu
{
   class committed_allocator : public igpu_allocator
   {
      public:
      committed_allocator(graphics_device_ptr&& dev_sp) :
         m_dev_sp(std::forward<graphics_device_ptr>(dev_sp)),
         m_totalSize_b(0)
      {

      }

      committed_allocator(const committed_allocator&) = delete;

      committed_allocator(committed_allocator&&) = default;

      virtual ~committed_allocator() = default;

      /*
       Committed resources need more info to allocate. This throws
       std::bad_alloc.
       */
      gpu_alloc_handle alloc(size_t bytes,
         const D3D12_RESOURCE_DESC& description,
         D3D12_RESOURCE_STATES initialState) override
      {
         throw std::bad_alloc{};
      }

      /*
       Allocates a committed resource on the GPU.
       */
      gpu_alloc_handle alloc(const D3D12_HEAP_PROPERTIES props,
                             D3D12_HEAP_FLAGS flgs,
                             const D3D12_RESOURCE_DESC& description,
                             D3D12_RESOURCE_STATES initialState,
                             const D3D12_CLEAR_VALUE* clear_p = nullptr)
      {
         std::lock_guard allocationLock{ m_allocationMutex };

         winrt::com_ptr<igpu_resource> resc;
         winrt::check_hresult(m_dev_sp->dev_3d()->CreateCommittedResource(
            &props, flgs, &description, initialState, clear_p,
            IID_PPV_ARGS(resc.put())));

         auto allocInfo = m_dev_sp->dev_3d()->GetResourceAllocationInfo(
            0, 1, &description);

         auto plusSize = allocInfo.SizeInBytes;
         auto p = m_resources.emplace(std::move(allocInfo), std::move(resc));
         if (!p.second)
         {
            throw std::bad_alloc();
         }

         m_totalSize_b += plusSize;
         return p.first->first;
      }

      virtual void free(gpu_alloc_handle hndl)
      {
         // Do not allow multiple threads to allocate or free at the same time.
         std::lock_guard allocationLock{ m_allocationMutex };

         auto subtractSize = m_resources.at(hndl).info.SizeInBytes;

         // Remove the resource pointer from the internal map. This calls the
         // destructor on the resource.
         m_resources.erase(hndl);

         m_totalSize_b -= subtractSize;
      }

      /*
       There is no fixed size. So this returns how much memory is currently
       used.
       */
      virtual size_t size() const noexcept
      {
         return used();
      }

      virtual size_t used() const noexcept
      {
         return m_totalSize_b;
      }

      virtual igpu_resource* resource(gpu_alloc_handle hndl)
      {
         if (m_resources.count(hndl) > 0)
         {
            return m_resources.at(hndl).resource_up.get();
         }

         return nullptr;
      }

      virtual const igpu_resource* resource(gpu_alloc_handle hndl) const
      {
         if (m_resources.count(hndl) > 0)
         {
            return m_resources.at(hndl).resource_up.get();
         }

         return nullptr;
      }

      /*
       Committed allocators do not have 1 central heap. This returns nullptr.
       */
      virtual igpu_heap* backing()
      {
         return nullptr;
      }

      /*
       Committed allocators do not have 1 central heap. This returns nullptr.
       */
      virtual const igpu_heap* backing() const
      {
         return nullptr;
      }

      private:
      struct resource_info
      {
         public:
         resource_info(D3D12_RESOURCE_ALLOCATION_INFO&& i,
                       winrt::com_ptr<igpu_resource>&& up) :
            info(std::forward<D3D12_RESOURCE_ALLOCATION_INFO>(i)),
            resource_up(std::forward<winrt::com_ptr<igpu_resource>>(up))
         {

         }

         resource_info(const resource_info&) = default;

         resource_info(resource_info&&) = default;

         ~resource_info() = default;

         D3D12_RESOURCE_ALLOCATION_INFO info;
         winrt::com_ptr<igpu_resource> resource_up;
      };

      /*
       Pointer to the graphics device. Use this to create resources.
       */
      graphics_device_ptr m_dev_sp;

      /*
       Used to block multiple threads from allocating simultaneously.
       */
      std::mutex m_allocationMutex;

      /*
       Maps handles to the actual resource.
       */
      std::unordered_map<gpu_alloc_handle, resource_info> m_resources;

      size_t m_totalSize_b;
   };

   using committed_allocator_ptr = typename std::shared_ptr<committed_allocator>;
}