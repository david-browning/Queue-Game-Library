#pragma once
#include "qgl_graphics_include.h"
#include "qgl_igpu_buffer.h"

namespace qgl::graphics
{
   class low::copy_command_list;

   /*
    You can create a vertex buffer without a command list but the GPU heap will be empty.
    To get the vertex data onto the GPU, queue this item in a copy_command_list.
    */
   template<typename VertexT>
   class vertex_buffer : public low::igpu_buffer<D3D12_SUBRESOURCE_DATA, D3D12_VERTEX_BUFFER_VIEW, d3d_resource>
   {
      friend class low::copy_command_list;

      public:
      using ResourceDescriptionT = D3D12_SUBRESOURCE_DATA;
      using ViewDescriptionT = D3D12_VERTEX_BUFFER_VIEW;

      vertex_buffer(winrt::com_ptr<d3d_device>& dev_p,
                    const std::vector<VertexT>& verts) :
         m_desc(),
         m_viewDesc(),
         m_vertices(verts),
         m_vertexBufferSize(sizeof(VertexT) * verts.size()),
         igpu_buffer(dev_p)
      {
         p_allocate();
      }

      vertex_buffer(winrt::com_ptr<d3d_device>& dev_p,
                    std::vector<VertexT>&& verts) :
         m_desc(),
         m_viewDesc(),
         m_vertices(verts),
         m_vertexBufferSize(sizeof(VertexT) * verts.size()),
         igpu_buffer(dev_p)
      {
         p_allocate();
      }

      vertex_buffer(const vertex_buffer& r) = delete;

      vertex_buffer(vertex_buffer&& r) = delete;

      virtual ~vertex_buffer()
      {
      }

      size_t stride() const
      {
         return sizeof(VertexT);
      }

      #pragma region Description Getters
      virtual ResourceDescriptionT description() const
      {
         return m_desc;
      }

      virtual ViewDescriptionT view() const
      {
         return m_viewDesc;
      }
      #pragma endregion


      private:

      void p_allocate()
      {
         //Initialize the description.
         m_desc.pData = m_vertices.data();
         m_desc.RowPitch = m_vertexBufferSize;
         m_desc.SlicePitch = m_desc.RowPitch;

         //Create the committed resource
         winrt::check_hresult(m_dev_p->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
            D3D12_HEAP_FLAG_NONE,
            &CD3DX12_RESOURCE_DESC::Buffer(m_vertexBufferSize),
            D3D12_RESOURCE_STATE_COPY_DEST,
            nullptr,
            IID_PPV_ARGS(m_resource.put())));

         //Initialize the vertex buffer view.
         m_viewDesc.BufferLocation = m_resource->GetGPUVirtualAddress();
         m_viewDesc.SizeInBytes = static_cast<UINT>(m_vertexBufferSize);
         m_viewDesc.StrideInBytes = static_cast<UINT>(sizeof(VertexT));
      }

      std::vector<VertexT> m_vertices;
      ResourceDescriptionT m_desc;
      ViewDescriptionT m_viewDesc;
      size_t m_vertexBufferSize;
   };
}