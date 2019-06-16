#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::content
{
   class light;
}

namespace qgl::content::loaders
{
   class QGL_GRAPHICS_API light_file_loader
   {
      public:
      light_file_loader(graphics::d3d_device* dev_p);

      std::unique_ptr<light> operator()(const icontent_file* f,
                                        const id_t newID);

      private:
      graphics::d3d_device* m_dev_p;
   };
}