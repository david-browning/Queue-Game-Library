#pragma once
#include "include/qgl_graphics_include.h"
#include "include/qgl_graphics_device.h"
#include "include/GPU/Memory/qgl_igpu_allocator.h"

namespace qgl::graphics::gpu
{
   class committed_allocator : public igpu_allocator
   {
      public:
      committed_allocator(graphics_device* dev_p,
                          const D3D12_HEAP_PROPERTIES& heapProps,
                          D3D12_HEAP_FLAGS flgs) :
         m_dev_p(dev_p),
         m_heapProps(heapProps),
         m_heapFlags(flgs),
         m_totalSize_b(0)
      {

      }

      committed_allocator(const committed_allocator&) = delete;

      committed_allocator(committed_allocator&& x) noexcept :
         igpu_allocator(std::move(x)),
         m_resources(std::move(x.m_resources)),
         m_dev_p(std::move(x.m_dev_p)),
         m_totalSize_b(x.m_totalSize_b),
         m_heapFlags(std::move(x.m_heapFlags)),
         m_heapProps(std::move(x.m_heapProps))
      {
         x.m_dev_p = nullptr;
         x.m_totalSize_b = static_cast<size_t>(-1);
      }

      virtual ~committed_allocator()
      {
         m_dev_p = nullptr;
         m_totalSize_b = static_cast<size_t>(-1);
      }

      /*
       Allocates a committed resource on the GPU.
       */
      gpu_alloc_handle alloc(const D3D12_RESOURCE_DESC& description,
                             D3D12_RESOURCE_STATES initialState) override
      {
         winrt::com_ptr<igpu_resource> resc;
         winrt::check_hresult(m_dev_p->dev_3d()->CreateCommittedResource(
            &m_heapProps, m_heapFlags, &description, initialState, nullptr,
            IID_PPV_ARGS(resc.put())));

         auto allocInfo = m_dev_p->dev_3d()->GetResourceAllocationInfo(
            0, 1, &description);

         auto plusSize = allocInfo.SizeInBytes;

         resource_info toEmplace{ std::move(allocInfo), std::move(resc) };
         auto ret = m_resources.alloc(std::move(toEmplace));

         m_totalSize_b += plusSize;
         return ret;
      }

      virtual void free(gpu_alloc_handle hndl)
      {
         auto subtractSize = m_resources.get(hndl).info.SizeInBytes;

         // Remove the resource pointer from the internal map. This calls the
         // destructor on the resource.
         m_resources.free(hndl);

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
         if (m_resources.allocated(hndl))
         {
            return m_resources.get(hndl).resource_up.get();
         }

         return nullptr;
      }

      virtual const igpu_resource* resource(gpu_alloc_handle hndl) const
      {
         if (m_resources.allocated(hndl))
         {
            return m_resources.get(hndl).resource_up.get();
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

         resource_info(resource_info&& x) noexcept :
            info(std::move(x.info)),
            resource_up(std::move(x.resource_up))
         {
            x.resource_up = nullptr;
         }

         ~resource_info() = default;

         D3D12_RESOURCE_ALLOCATION_INFO info;
         winrt::com_ptr<igpu_resource> resource_up = nullptr;
      };

      /*
       Stores handles to the GPU allocations
       */
      qgl::handle_map<resource_info> m_resources;

      /*
       Pointer to the graphics device. Use this to create resources.
       */
      graphics_device* m_dev_p = nullptr;
      
      size_t m_totalSize_b = static_cast<size_t>(-1);
      D3D12_HEAP_FLAGS m_heapFlags;
      D3D12_HEAP_PROPERTIES m_heapProps;
   };
}