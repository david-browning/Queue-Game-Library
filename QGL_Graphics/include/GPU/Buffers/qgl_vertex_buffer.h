#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/qgl_sprite_vert.h"
#include "include/GPU/qgl_geom2d_vert.h"
#include "include/GPU/Buffers/qgl_igpu_buffer.h"
#include "include/GPU/Memory/qgl_igpu_allocator.h"

namespace qgl::graphics::gpu
{
   /*
    This does not upload the index buffer to the GPU. Pass this to an
    "upload_buffer" to upload the data this manages.
    */
   template<typename VertexT, D3D_PRIMITIVE_TOPOLOGY TopologyT>
   class vertex_buffer final : public igpu_buffer<D3D12_SUBRESOURCE_DATA,
                                                  D3D12_VERTEX_BUFFER_VIEW,
                                                  igpu_resource>
   {
      public:
      using ResDescT = typename D3D12_SUBRESOURCE_DATA;
      using ViewDescT = typename D3D12_VERTEX_BUFFER_VIEW;
      using vertex_data = typename std::vector<VertexT>;

      /*
       This does not own the allocator pointer. Do not free the allocator or let
       it go out of scope before destroying this.
       */
      vertex_buffer(igpu_allocator* allocator_p,
                    const vertex_data& vertices) :
         m_verts(vertices),
         m_allocator_p(allocator_p),
         igpu_buffer()
      {
         construct();
      }

      /*
       This does not own the allocator pointer. Do not free the allocator or let
       it go out of scope before destroying this.
       */
      vertex_buffer(igpu_allocator* allocator_p,
                    const VertexT* const vertices,
                    size_t numVertices) :
         m_allocator_p(allocator_p),
         igpu_buffer()
      {
         m_verts.resize(numVertices);
         memcpy(m_verts.data(), vertices, sizeof(VertexT) * numVertices);
         construct();
      }

      vertex_buffer(const vertex_buffer&) = delete;

      vertex_buffer(vertex_buffer&& x) noexcept :
         m_allocator_p(std::move(x.m_allocator_p)),
         m_verts(std::move(x.m_verts)),
         m_alloc_h(std::move(x.m_alloc_h)),
         m_viewDescription(std::move(x.m_viewDescription)),
         m_resDescription(std::move(x.m_resDescription)),
         m_state(std::move(x.m_state))
      {
         x.m_allocator_p = nullptr;
      }

      virtual ~vertex_buffer()
      {
         if (m_allocator_p)
         {
            m_allocator_p->free(m_alloc_h);
            m_allocator_p = nullptr;
         }
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

      virtual gpu_alloc_handle alloc_handle() const
      {
         return m_alloc_h;
      }

      virtual const igpu_resource* get() const
      {
         return m_allocator_p->get(m_alloc_h);
      }

      virtual igpu_resource* get()
      {
         return m_allocator_p->get(m_alloc_h);
      }

      virtual size_t size() const noexcept
      {
         return sizeof(VertexT) * m_verts.size();
      }

      size_t count() const noexcept
      {
         return m_verts.size();
      }

      virtual D3D12_RESOURCE_STATES state() const noexcept
      {
         return m_state;
      }

      virtual void state(D3D12_RESOURCE_STATES s) noexcept
      {
         m_state = s;
      }

      private:
      void construct()
      {
         //Initialize the description.
         m_resDescription.pData = m_verts.data();
         m_resDescription.RowPitch = size();
         m_resDescription.SlicePitch = m_resDescription.RowPitch;

         // Allocate a buffer
         m_alloc_h = m_allocator_p->alloc(CD3DX12_RESOURCE_DESC::Buffer(size()));

         //Initialize the vertex buffer view.
         m_viewDescription.BufferLocation = get()->GetGPUVirtualAddress();
         m_viewDescription.SizeInBytes = static_cast<UINT>(size());
         m_viewDescription.StrideInBytes = static_cast<UINT>(sizeof(VertexT));
      }

      vertex_data m_verts;
      ViewDescT m_viewDescription;
      ResDescT m_resDescription;
      igpu_allocator* m_allocator_p = nullptr;
      gpu_alloc_handle m_alloc_h = static_cast<gpu_alloc_handle>(-1);
      D3D12_RESOURCE_STATES m_state = D3D12_RESOURCE_STATE_COPY_DEST;
   };
}