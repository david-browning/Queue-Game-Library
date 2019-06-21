#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::content
{
   class blender;
}

namespace qgl::content::loaders
{
   class QGL_GRAPHICS_API blender_file_loader
   {
      public:
      std::unique_ptr<blender> operator()(const icontent_file* f,
                                          id_t newID) const;
   };
}