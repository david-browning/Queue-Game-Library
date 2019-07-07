#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Buffers/igpu_buffer.h"
#include "include/Interfaces/qgl_igraphics_device.h"

namespace qgl::graphics::gpu::buffers
{
   /*
    You can create a vertex buffer without a command list but the GPU heap will 
    be empty.
    To get the vertex data onto the GPU, queue this item in a copy_command_list.
    */
   template<typename VertexT, D3D_PRIMITIVE_TOPOLOGY TopologyT>
   class vertex_buffer : public igpu_buffer<D3D12_SUBRESOURCE_DATA, 
      D3D12_VERTEX_BUFFER_VIEW, 
      d3d_resource>
   {
      public:
      using ResourceDescriptionT = D3D12_SUBRESOURCE_DATA;
      using ViewDescriptionT = D3D12_VERTEX_BUFFER_VIEW;

      vertex_buffer(static_ptr_ref<graphics::igraphics_device> gdev,
                    const VertexT* vertices,
                    size_t numVertices) :
         m_desc(),
         m_viewDesc(),
         m_vertexBufferSize(sizeof(VertexT) * numVertices)
      {
         m_vertices.resize(numVertices);
         MemoryCopy(m_vertices.data(), vertices, numVertices);
         construct(gdev);
      }

      vertex_buffer(static_ptr_ref<graphics::igraphics_device> gdev,
                    std::vector<VertexT>& vertices) :
         m_desc(),
         m_viewDesc(),
         m_vertexBufferSize(sizeof(VertexT) * vertices.size()),
         m_vertices(vertices)
      {
         construct(gdev);
      }

      vertex_buffer(const vertex_buffer&) = delete;

      vertex_buffer(vertex_buffer&&) = default;

      virtual ~vertex_buffer() noexcept = default;

      constexpr size_t stride() const noexcept
      {
         return sizeof(VertexT);
      }

      virtual const ResourceDescriptionT* description() const
      {
         return &m_desc;
      }

      virtual const ViewDescriptionT* view() const
      {
         return &m_viewDesc;
      }

      D3D_PRIMITIVE_TOPOLOGY topology() const noexcept
      {
         return TopologyT;
      }

      private:
      void construct(static_ptr_ref<graphics::igraphics_device> gdev)
      {
         //Initialize the description.
         m_desc.pData = m_vertices.data();
         m_desc.RowPitch = m_vertexBufferSize;
         m_desc.SlicePitch = m_desc.RowPitch;

         //Create the committed resource
         winrt::check_hresult(gdev->d3d12_device()->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
            D3D12_HEAP_FLAG_NONE,
            &CD3DX12_RESOURCE_DESC::Buffer(m_vertexBufferSize),
            D3D12_RESOURCE_STATE_COPY_DEST,
            nullptr,
            IID_PPV_ARGS(put())));

         //Initialize the vertex buffer view.
         m_viewDesc.BufferLocation = get()->GetGPUVirtualAddress();
         m_viewDesc.SizeInBytes = static_cast<UINT>(m_vertexBufferSize);
         m_viewDesc.StrideInBytes = static_cast<UINT>(sizeof(VertexT));
      }

      std::vector<VertexT> m_vertices;
      ResourceDescriptionT m_desc;
      ViewDescriptionT m_viewDesc;
      size_t m_vertexBufferSize;
   };
}