#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::content
{
   class sampler;
}

namespace qgl::content::loaders
{
   extern QGL_GRAPHICS_API std::unique_ptr<sampler>
      qgl_load_sampler_file(const icontent_file* f,
                            const id_t newID);
}