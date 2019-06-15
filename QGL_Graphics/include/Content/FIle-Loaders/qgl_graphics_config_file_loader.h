#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::content
{
   class graphics_config;
}

namespace qgl::content::loaders
{
   extern QGL_GRAPHICS_API std::unique_ptr<graphics_config>
      qgl_load_graphics_config_file(const icontent_file* f,
                                    const id_t newID);
}