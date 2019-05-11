#pragma once
#include "qgl_graphics_include.h"
#include "qgl_ibindable.h"
#include "qgl_imappable.h"
#include "qgl_igpu_buffer.h"

namespace qgl::graphics
{
   template<typename T>
   class const_buffer :
      public low::igpu_buffer<int, D3D12_CONSTANT_BUFFER_VIEW_DESC, d3d_resource>,
      public low::imappable<T>
   {
      public:
      using ResourceDescriptionT = int;
      using ViewDescriptionT = D3D12_CONSTANT_BUFFER_VIEW_DESC;
      const_buffer(winrt::com_ptr<d3d_device>& dev_p) :
         imappable(),
         igpu_buffer(dev_p)
      {
         p_allocate();
      }

      const_buffer(const const_buffer& r) = delete;

      const_buffer(const_buffer&& r) = delete;

      virtual ~const_buffer()
      {

      }

      virtual ResourceDescriptionT description() const
      {
         throw std::runtime_error("const_buffers do not have a resource description.");
      }

      /*
       Returns a view that can be bound to the shader pipeline.
       */
      virtual ViewDescriptionT view() const
      {
         return m_viewDescription;
      }

      private:
      void p_allocate()
      {
         auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
         auto rscDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(T));
         winrt::check_hresult(m_dev_p->CreateCommittedResource(
            &heapProps,
            D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS,
            &rscDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(m_resource.put())));

         m_viewDescription.SizeInBytes = sizeof(T);
         m_viewDescription.BufferLocation = m_resource->GetGPUVirtualAddress();
      }

      virtual void p_map()
      {
         //nullptr read range indicates the entire sub-resource might be read by the CPU. 
         //It is valid to specify the CPU won't read any data by passing a range where 
         //End is less than or equal to Begin.
         winrt::check_hresult(m_resource->Map(0, nullptr, reinterpret_cast<void**>(&m_buffer)));
      }

      virtual void p_unmap()
      {
         //nullptr read range indicates the entire sub-resource might have been modified by 
         //the CPU.
         m_resource->Unmap(0, nullptr);
      }

      /*
       Description of the view that gets bound to the shader pipeline.
       */
      ViewDescriptionT m_viewDescription;
   };
}