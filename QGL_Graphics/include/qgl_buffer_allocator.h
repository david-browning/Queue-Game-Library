#pragma once
#include "qgl_graphics_include.h"
#include "qgl_memory.h"
#include "qgl_imem_allocator.h"

namespace qgl::graphics::low
{
   template<size_t UAlignment, 
      D3D12_RESOURCE_STATES CreationState = D3D12_RESOURCE_STATE_COPY_DEST, 
      D3D12_HEAP_TYPE  HeapType = D3D12_HEAP_TYPE_UPLOAD>
   class LIB_EXPORT buffer_allocator : public imemory_allocator<UAlignment>
   {
      public:
      buffer_allocator(const winrt::com_ptr<d3d_device>& dev_p) :
         m_dev(dev_p),
         m_heapMap(),
         imemory_allocator()
      {

      }

      /*
       imemory_allocator deleted this.
       */
      buffer_allocator(const buffer_allocator& r) = delete;

      /*
       imemory_allocator deleted this.
       */
      buffer_allocator(buffer_allocator&& r) = delete;

      virtual ~buffer_allocator()
      {
         //The heap map is destructed, which destructs each com_ptr in it.
         //When a com_ptr is destructed, it releases the resource the pointer references.
         clear();
      }

      /*
       Allocates a D3D resource and heap for it. Additional bytes may be added to properly 
       align the buffer. The resource's initial state is 
       determined by the class's templates.
       */
      virtual D3D12_GPU_VIRTUAL_ADDRESS allocate(size_t buffSize)
      {
         //Align the buffer size.
         auto alignedSize = qgl::graphics::low::align<UAlignment, size_t>(buffSize);

         //Create a new resource.
         winrt::com_ptr<d3d_resource> newResource;
         auto heapProps = CD3DX12_HEAP_PROPERTIES(HeapType);
         auto rescDesc = CD3DX12_RESOURCE_DESC::Buffer(alignedSize);
         winrt::check_hresult(m_dev->CreateCommittedResource(
            &heapProps,
            D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
            &rescDesc,
            CreationState,
            nullptr,
            IID_PPV_ARGS(newResource.put())));

         //Get the resource's GPU address.
         auto gpuAddress = newResource->GetGPUVirtualAddress();

         //Add the address/buffer to the heap map.
         m_heapMap[gpuAddress] = newResource;

         return gpuAddress;
      }

      virtual void deallocate(const D3D12_GPU_VIRTUAL_ADDRESS& addr)
      {
         p_free_buffer(addr);
      }
           
      /*
       Returns a reference to the resource that was allocated at the given GPU address.
       Throws an exception if the address is not mapped.
       */
      winrt::com_ptr<d3d_resource> buffer(const D3D12_GPU_VIRTUAL_ADDRESS& addr) const
      {
         return m_heapMap.at(addr);
      }

      void clear()
      {
         m_heapMap.clear();
      }

      private:
 
      inline void p_free_buffer(const D3D12_GPU_VIRTUAL_ADDRESS& addr)
      {
         //Remove the resource from the heap map.
         //This calls the resource's memory because its a smart pointer.
         m_heapMap.erase(addr);
      }

      #pragma warning(push)
      #pragma warning(disable: 4251)
      /*
       Map each created resource to the GPU address where it exists.
       */
      std::unordered_map<D3D12_GPU_VIRTUAL_ADDRESS, winrt::com_ptr<d3d_resource>> m_heapMap;
      #pragma warning(pop)

      winrt::com_ptr<d3d_device> m_dev;
   };
}