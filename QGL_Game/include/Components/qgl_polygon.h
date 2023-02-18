#pragma once
#include "include/qgl_game_include.h"

namespace qgl::components
{
   static constexpr guid POLYGON_GUID{ "9CA1E5C4C37F4542AE1CFA0A72B4278E" };

   template<class VertexT>
   class polygon : public game_component<polygon<VertexT>>
   {
      public:
      polygon(VertexT* verts,
              qgl::graphics::gpu::igpu_allocator* allocator_p,
              game_update_functor<polygon<VertexT>> f) :
         m_geom(verts, 3, allocator_p),
         game_component<polygon<VertexT>>(POLYGON_GUID, f)
      {

      }

      polygon(const std::array<VertexT, 3>& verts,
              qgl::graphics::gpu::igpu_allocator* allocator_p,
              game_update_functor<polygon<VertexT>> f) :
         m_geom(verts.data(), verts.size(), allocator_p),
         game_component<polygon<VertexT>>(POLYGON_GUID, f)
      {
      }

      polygon(const qgl::fixed_buffer<VertexT, 3>& verts,
              qgl::graphics::gpu::igpu_allocator* allocator_p,
              game_update_functor<polygon<VertexT>> f) :
         m_geom(verts.data(), verts.size(), allocator_p),
         game_component<polygon<VertexT>>(POLYGON_GUID, f)
      {

      }

      polygon(const polygon&) = delete;

      polygon(polygon&&) noexcept = default;

      virtual ~polygon() noexcept = default;

      private:
      qgl::graphics::gpu::vertex_buffer<
         VertexT, D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP> m_geom;
   };
}