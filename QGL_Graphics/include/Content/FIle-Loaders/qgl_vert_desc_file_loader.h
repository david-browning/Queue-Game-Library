#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::content
{
   class vertex_description;
}

namespace qgl::content::loaders
{
   class QGL_GRAPHICS_API vert_desc_file_loader
   {
      public:
      std::unique_ptr<vertex_description> operator()(const icontent_file* f,
                                                     const id_t newID) const;
   };
}