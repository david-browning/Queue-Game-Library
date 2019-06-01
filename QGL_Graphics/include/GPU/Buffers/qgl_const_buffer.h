#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Buffers/igpu_buffer.h"
#include "include/GPU/Buffers/qgl_imappable.h"

namespace qgl::graphics::gpu::buffers
{
   template<typename T>
   class const_buffer :
      public imappable<T>,
      public igpu_buffer<nullptr_t,
      D3D12_CONSTANT_BUFFER_VIEW_DESC,
      d3d_resource>
   {
      public:
      using ViewDescriptionT = D3D12_CONSTANT_BUFFER_VIEW_DESC;
      using ResourceDescriptionT = nullptr_t;

      const_buffer(graphics_device* gdev) :
         m_viewDescription()
      {
         construct(gdev);
      }

      /*
       Returns a description of the resource.
       */
      virtual const ResourceDescriptionT* description() const
      {
         throw std::runtime_error(
            "const_buffers do not have a resource description.");
      }

      /*
       Returns a view pointer that can be bound to the shader pipeline.
       */
      virtual const ViewDescriptionT* view() const
      {
         return &m_viewDescription;
      }

      private:

      void construct(graphics_device* gdev)
      {
         auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
         auto rscDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(T));
         winrt::check_hresult(gdev->d3d12_device()->CreateCommittedResource(
            &heapProps,
            D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS,
            &rscDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(put())));

         m_viewDescription.SizeInBytes = sizeof(T);
         m_viewDescription.BufferLocation = get()->GetGPUVirtualAddress();
      }

      virtual void p_map()
      {
         //nullptr read range indicates the entire sub-resource might be read
         //by the CPU. 
         //It is valid to specify the CPU won't read any data by passing a 
         //range where End is less than or equal to Begin.
         auto ptr = mapping();
         winrt::check_hresult(get()->Map(0,
                                         nullptr,
                                         reinterpret_cast<void**>(&ptr)));
      }

      virtual void p_unmap()
      {
         //nullptr read range indicates the entire sub-resource might have 
         //been modified by the CPU.
         get()->Unmap(0, nullptr);
      }

      /*
       Description of the view that gets bound to the shader pipeline.
       */
      ViewDescriptionT m_viewDescription;
   };
}