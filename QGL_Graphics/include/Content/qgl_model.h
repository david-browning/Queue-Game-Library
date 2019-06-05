#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Buffers/qgl_vertex_buffer.h"
#include "include/GPU/Buffers/qgl_index_buffer.h"
#include "include/GPU/Buffers/qgl_vertex_buffer.h"
#include "include/GPU/Buffers/qgl_index_buffer.h"
#include "include/GPU/Descriptors/qgl_descriptor_table.h"
using namespace qgl::graphics::gpu::buffers;

namespace qgl::content
{
   /*
    A model is geometry that can be rendered. Models have a vertex, index, and
    position buffer.
    */
   template<typename VertexT, D3D_PRIMITIVE_TOPOLOGY TopologyT, typename IndexT>
   class model
   {
      public:
      model(const VertexT* vertData,
            size_t vertCount,
            const IndexT* indexData,
            size_t indexCount,
            graphics::graphics_device* gdev) :
         m_idxBuffer(gdev, indexData, indexCount),
         m_vertBuffer(gdev, vertData, vertCount)
      {

      }

      model(const model&) = default;

      model(model&&) = default;

      virtual ~model() noexcept = default;

      /*
       Returns a pointer to the index buffer.
       */
      const index_buffer<IndexT>* idx_buffer() const noexcept
      {
         return &m_idxBuffer;
      }

      /*
       Returns a pointer to the vertex buffer.
       */
      const vertex_buffer<VertexT, TopologyT>* vert_buffer() const noexcept
      {
         return &m_vertBuffer;
      }

      /*
       Returns a pointer to the vertex buffer view used by command lists.
       */
      const D3D12_VERTEX_BUFFER_VIEW* vertex_view() const noexcept
      {
         return m_vertBuffer.view();
      }

      /*
       Returns a pointer to the index buffer view used by command lists.
       */
      const D3D12_INDEX_BUFFER_VIEW* index_view() const noexcept
      {
         return m_idxBuffer.view();
      }

      private:
      index_buffer<IndexT> m_idxBuffer;
      vertex_buffer<VertexT, TopologyT> m_vertBuffer;
   };
}