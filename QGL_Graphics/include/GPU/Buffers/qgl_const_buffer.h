#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Buffers/qgl_igpu_buffer.h"
#include "include/GPU/Buffers/qgl_imappable.h"
#include "include/GPU/Memory/qgl_igpu_allocator.h"

namespace qgl::graphics::gpu
{
   /*
    Creates a const buffer.
    This buffer does not upload data to the GPU.
    Either map the data or pass this to an upload buffer.

    Uses an allocator to allocate memory on the GPU. When this object is
    destroyed, it releases its resources from the allocator.
    */
   template<class T>
   class const_buffer : public igpu_buffer<
      nullptr_t,
      D3D12_CONSTANT_BUFFER_VIEW_DESC,
      igpu_resource>,
      public imappable<T>
   {
      public:
      using ViewDescT = D3D12_CONSTANT_BUFFER_VIEW_DESC;
      using ResDescT = nullptr_t;
      using ResourceT = typename igpu_resource;

      /*
       This does not own the allocator pointer. Do not free the allocator or let
       it go out of scope before destroying this.
       */
      const_buffer(igpu_allocator* allocator_sp) :
         m_allocator_p(allocator_sp),
         igpu_buffer()
      {
         const auto sz = sizeof(T);
         auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
         auto rscDesc = CD3DX12_RESOURCE_DESC::Buffer(sz);
         m_alloc_h = m_allocator_p->alloc(
            rscDesc, D3D12_RESOURCE_STATE_GENERIC_READ);

         auto res_p = allocator_sp->resource(m_alloc_h);
         m_viewDescription.SizeInBytes = sz;
         m_viewDescription.BufferLocation = res_p->GetGPUVirtualAddress();
      }

      const_buffer(const const_buffer&) = delete;

      const_buffer(const_buffer&& x) noexcept :
         m_allocator_p(x.m_allocator_p),
         m_alloc_h(x.m_alloc_h),
         m_viewDescription(x.m_viewDescription),
         m_buffer_p(m_buffer_p),
         igpu_buffer()
      {
         x.unmap();
         x.m_allocator_p = nullptr;
      }

      virtual ~const_buffer()
      {
         if (m_allocator_p)
         {
            unmap();
            m_allocator_p->free(m_alloc_h);
            m_allocator_p = nullptr;
         }
      }

      virtual const ResDescT* description() const
      {
         throw std::runtime_error(
            "const_buffers do not have a resource description.");
      }

      virtual const ViewDescT* view() const
      {
         return &m_viewDescription;
      }

      virtual gpu_alloc_handle alloc_handle() const noexcept
      {
         return m_alloc_h;
      }

      virtual const ResourceT* get() const
      {
         return m_allocator_p->resource(m_alloc_h);
      }

      virtual size_t size() const noexcept
      {
         return sizeof(T);
      }

      virtual ResourceT* get()
      {
         return m_allocator_p->resource(m_alloc_h);
      }

      virtual void map()
      {
         //nullptr read range indicates the entire sub-resource might be read
         //by the CPU. 
         //It is valid to specify the CPU won't read any data by passing a 
         //range where End is less than or equal to Begin.
         if (!mapped())
         {
            auto resource = m_allocator_p->resource(m_alloc_h);
            check_result(resource->Map(0, nullptr,
               reinterpret_cast <void**>(&m_buffer_p)));
         }
      }

      virtual void unmap()
      {
         //nullptr read range indicates the entire sub-resource might have 
         //been modified by the CPU.
         if (mapped())
         {
            auto resource = m_allocator_p->resource(m_alloc_h);
            resource->Unmap(0, nullptr);
            m_buffer_p = nullptr;
         }
      }

      virtual const T* mapping() const
      {
         return m_buffer_p;
      }

      virtual T* mapping()
      {
         return m_buffer_p;
      }

      virtual bool mapped() const noexcept
      {
         return m_buffer_p != nullptr;
      }

      private:

      igpu_allocator* m_allocator_p = nullptr;

      gpu_alloc_handle m_alloc_h = static_cast<gpu_alloc_handle>(-1);

      /*
       Description of the view that gets bound to the shader pipeline.
       */
      ViewDescT m_viewDescription;

      T* m_buffer_p = nullptr;
   };
}