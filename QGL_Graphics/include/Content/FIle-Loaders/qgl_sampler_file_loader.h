#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::content
{
   class sampler;
}

namespace qgl::content::loaders
{
   class QGL_GRAPHICS_API sampler_file_loader
   {
      public:
      std::unique_ptr<sampler> operator()(const icontent_file* f,
                                          const id_t newID);
   };
}