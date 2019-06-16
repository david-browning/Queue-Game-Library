#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::content
{
   class rasterizer;
}

namespace qgl::content::loaders
{
   extern QGL_GRAPHICS_API std::unique_ptr<rasterizer>
      qgl_load_rasterizer_file(const icontent_file* f,
                               const id_t newID);
}