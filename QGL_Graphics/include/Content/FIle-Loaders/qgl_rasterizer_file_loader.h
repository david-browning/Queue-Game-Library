#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::content
{
   class rasterizer;
}

namespace qgl::content::loaders
{
   class QGL_GRAPHICS_API rasterizer_file_loader
   {
      public:
      std::unique_ptr<rasterizer> operator()(const icontent_file* f,
                                             const id_t newID);
   };
}