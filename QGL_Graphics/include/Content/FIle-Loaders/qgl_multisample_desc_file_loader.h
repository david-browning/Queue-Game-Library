#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/qgl_multisample_desc.h"

namespace qgl::content::loaders
{
   class QGL_GRAPHICS_API multisample_desc_file_loader
   {
      public:
      std::unique_ptr<multisample_desc> operator()(const icontent_file* f,
                                                   id_t newID) const;
   };
}