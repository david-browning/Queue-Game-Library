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
   template<class ViewDescT>
   class upload_buffer : public igpu_buffer<
      D3D12_SUBRESOURCE_DATA,
      ViewDescT,
      gpu_resource>
   {
      public:
      template<class T>
      upload_buffer(const const_buffer<T>& buff, const committed_allocator a);

      template<class VertexT, D3D_PRIMITIVE_TOPOLOGY TopologyT>
      upload_buffer(const vertex_buffer<VertexT, TopologyT>& buff, 
                    const committed_allocator a);

      template<typename IndexT>
      upload_buffer(const index_buffer<IndexT>& buff, 
                    const committed_allocator a);

      upload_buffer(const texture_buffer& buff, const committed_allocator a);

      upload_buffer(const upload_buffer&) = delete;

      upload_buffer(upload_buffer&&);

      virtual ~upload_buffer();

      virtual const D3D12_SUBRESOURCE_DATA* description() const;

      virtual const ViewDescT* view() const;

      virtual size_t size() const noexcept;

      virtual gpu_alloc_handle alloc_handle() const noexcept;

      virtual const gpu_resource* get() const;

      virtual gpu_resource* get();

      private:
   };
}