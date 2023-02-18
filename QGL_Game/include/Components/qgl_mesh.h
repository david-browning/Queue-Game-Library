#pragma once
#include "include/qgl_game_include.h"

namespace qgl::components
{
   static constexpr guid MODEL_MESH_GUID{ "7115E74E944C4222B4894B79C83B29A3" };

   template<class VertexT, typename IndexT>
   class model_mesh : public game_component<model_mesh<VertexT, IndexT>>
   {
      public:
      using vert_buffer_type = typename qgl::graphics::gpu::vertex_buffer<
         VertexT, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST>;
      using idx_buffer_type = typename qgl::graphics::gpu::index_buffer<IndexT>;

      model_mesh(const VertexT* const verts,
                 size_t numVerts,
                 const IndexT* const indices,
                 size_t numIndices,
                 qgl::graphics::gpu::igpu_allocator* allocator_p,
                 game_update_functor<model_mesh<VertexT, IndexT>> f) :
         m_vBuffer(verts, numVerts, allocator_p),
         m_iBuffer(indices, numIndices, allocator_p),
         game_component<model_mesh<VertexT, IndexT>>(MODEL_MESH_GUID, f)
      {

      }

      model_mesh(const std::vector<VertexT>& verts,
                 const std::vector<IndexT>& indices,
                 qgl::graphics::gpu::igpu_allocator* allocator_p,
                 game_update_functor<model_mesh<VertexT, IndexT>> f) :
         m_vBuffer(verts, allocator_p),
         m_iBuffer(indices, allocator_p),
         game_component<model_mesh<VertexT, IndexT>>(MODEL_MESH_GUID, f)
      {

      }

      model_mesh(const model_mesh&) = delete;

      model_mesh(model_mesh&&) noexcept = default;

      virtual ~model_mesh() noexcept = default;

      vert_buffer_type& vbuffer() noexcept
      {
         return m_vBuffer;
      }

      const vert_buffer_type& vbuffer() const noexcept
      {
         return m_vBuffer;
      }

      idx_buffer_type& ibuffer() noexcept
      {
         return m_iBuffer;
      }

      const idx_buffer_type& ibuffer() const noexcept
      {
         return m_iBuffer;
      }

      private:
      vert_buffer_type m_vBuffer;
      idx_buffer_type m_iBuffer;
   };
}