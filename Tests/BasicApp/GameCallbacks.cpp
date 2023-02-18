#include "pch.h"

void consoleCallback(const std::string&)
{

}

void dsvAllocCallback(qgl::graphics::gpu::igpu_allocator* alloc_p)
{
   auto& info = alloc_p->stats();
}

qgl::result_t cubeMeshCallback(
   qgl::components::model_mesh<qgl::graphics::gpu::geom3d_vert, uint16_t>&,
   qgl::components::game_context&)
{
   return S_OK;
}

qgl::result_t cameraCallback(qgl::components::camera&,
                             qgl::components::game_context&)
{
   return S_OK;
}