#pragma once
#include "include/qgl_game_include.h"

namespace qgl::components
{
   template<class VertexT>
   class polygon : public game_component<polygon<VertexT>>
   {
      public:

      private:
      qgl::graphics::gpu::vertex_buffer<
         VertexT, D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP> m_geom;
   };
}