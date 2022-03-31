#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Buffers/qgl_igpu_buffer.h"
#include "include/GPU/Memory/qgl_igpu_allocator.h"

namespace qgl::graphics::gpu
{
   /*
    This does not upload the index buffer to the GPU. Pass this to an
    "upload_buffer" to upload the data this manages.
    */
   template<typename VertexT, D3D_PRIMITIVE_TOPOLOGY TopologyT>
   class vertex_buffer : public igpu_buffer<D3D12_SUBRESOURCE_DATA,
      D3D12_VERTEX_BUFFER_VIEW,
      igpu_resource>
   {
      public:
      using ResDescT = D3D12_SUBRESOURCE_DATA;
      using ViewDescT = D3D12_VERTEX_BUFFER_VIEW;
      using vertex_data = typename std::vector<VertexT>;

      vertex_buffer(gpu_allocator_ptr&& allocator_sp,
                    vertex_data&& vertices) :
         m_verts(std::forward<vertex_data>(vertices)),
         m_allocator_sp(std::forward<gpu_allocator_ptr>(allocator_sp))
      {
         construct();
      }

      vertex_buffer(gpu_allocator_ptr&& allocator,
                    const VertexT* const vertices,
                    size_t numVertices) :
         m_allocator_sp(std::forward<gpu_allocator_ptr>(allocator))
      {
         m_verts.resize(numVertices);
         memcpy(m_verts.data(), vertices, sizeof(VertexT) * numVertices);
         construct();
      }

      vertex_buffer(const vertex_buffer&) = delete;

      vertex_buffer(vertex_buffer&&) = default;

      virtual ~vertex_buffer()
      {
         m_allocator_sp->free(m_alloc_h);
      }

      constexpr size_t stride() const noexcept
      {
         return sizeof(VertexT);
      }

      constexpr D3D_PRIMITIVE_TOPOLOGY topology() const noexcept
      {
         return TopologyT;
      }

      virtual const ResDescT* description() const
      {
         return &m_resDescription;
      }

      virtual const ViewDescT* view() const
      {
         return &m_viewDescription;
      }

      virtual gpu_alloc_handle alloc_handle() const noexcept
      {
         return m_alloc_h;
      }

      virtual const igpu_resource* get() const
      {
         return m_allocator_sp->resource(m_alloc_h);
      }

      virtual igpu_resource* get()
      {
         return m_allocator_sp->resource(m_alloc_h);
      }

      virtual size_t size() const noexcept
      {
         return sizeof(VertexT) * m_verts.size();
      }

      private:
      void construct()
      {
         //Initialize the description.
         m_resDescription.pData = m_verts.data();
         m_resDescription.RowPitch = size();
         m_resDescription.SlicePitch = m_resDescription.RowPitch;

         // TODO: if allocating index buffers fails, may need to restrict this 
         // to use a committed resource allocator_sp.
         //winrt::check_hresult(gdev->d3d12_device()->CreateCommittedResource(
         //   &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
         //   D3D12_HEAP_FLAG_NONE,
         //   &CD3DX12_RESOURCE_DESC::Buffer(m_vertexBufferSize),
         //   D3D12_RESOURCE_STATE_COPY_DEST,
         //   nullptr,
         //   IID_PPV_ARGS(put())));

         m_alloc_h = m_allocator_sp->alloc(
            size(),
            0,
            CD3DX12_RESOURCE_DESC::Buffer(size()),
            D3D12_RESOURCE_STATE_COPY_DEST);

         //Initialize the vertex buffer view.
         m_viewDescription.BufferLocation = get()->GetGPUVirtualAddress();
         m_viewDescription.SizeInBytes = static_cast<UINT>(size());
         m_viewDescription.StrideInBytes = static_cast<UINT>(sizeof(VertexT));
      }

      vertex_data m_verts;
      gpu_allocator_ptr m_allocator_sp;
      gpu_alloc_handle m_alloc_h;
      ViewDescT m_viewDescription;
      ResDescT m_resDescription;
   };
}