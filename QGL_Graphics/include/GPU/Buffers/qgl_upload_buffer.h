#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Memory/qgl_committed_allocator.h"
#include "include/GPU/Buffers/qgl_igpu_buffer.h"
#include "include/GPU/Memory/qgl_igpu_allocator.h"
#include "include/GPU/Buffers/qgl_const_buffer.h"
#include "include/GPU/Buffers/qgl_index_buffer.h"
#include "include/GPU/Buffers/qgl_vertex_buffer.h"
#include "include/GPU/Buffers/qgl_texture_buffer.h"

namespace qgl::graphics::gpu
{
   class upload_buffer : public igpu_buffer<D3D12_SUBRESOURCE_DATA,
                                            nullptr_t,
                                            igpu_resource>
   {
      public:
      template<class VertexT, D3D_PRIMITIVE_TOPOLOGY TopologyT>
      upload_buffer(
         committed_allocator<D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_HEAP_TYPE_UPLOAD>* allocator_p,
         const vertex_buffer<VertexT, TopologyT>& buff) :
         m_allocator_p(allocator_p)
      {
         m_desc = *buff.description();
         m_resourceSize = buff.size();

         m_alloc_h = m_allocator_p->alloc(CD3DX12_RESOURCE_DESC::Buffer(size()));
      }

      upload_buffer(const upload_buffer&) = delete;

      upload_buffer(upload_buffer&&) = default;

      virtual ~upload_buffer() noexcept = default;

      virtual const D3D12_SUBRESOURCE_DATA* description() const
      {
         return &m_desc;
      }

      virtual const nullptr_t* view() const
      {
         throw std::runtime_error(
            "upload buffers do not have a view description.");
      }

      virtual size_t size() const noexcept
      {
         return m_resourceSize;
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

      virtual D3D12_RESOURCE_STATES state() const noexcept
      {
         return D3D12_RESOURCE_STATE_COMMON;
      }

      virtual void state(D3D12_RESOURCE_STATES) noexcept
      {
         
      }

      private:
      D3D12_SUBRESOURCE_DATA m_desc;
      size_t m_resourceSize;
      igpu_allocator* m_allocator_p;
      gpu_alloc_handle m_alloc_h = static_cast<gpu_alloc_handle>(-1);
   };
}