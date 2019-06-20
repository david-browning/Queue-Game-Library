#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::content
{
   class shader;
}

namespace qgl::content::loaders
{
   class QGL_GRAPHICS_API shader_file_loader
   {
      public:
      std::unique_ptr<shader> operator()(const icontent_file* f,
                                         const id_t newID) const;
   };
}