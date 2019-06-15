#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::content
{
   class brush;
}

namespace qgl::content::loaders
{
   /*
    Functor used to load a brush from a content file.
    */
   class QGL_GRAPHICS_API brush_file_loader
   {
      public:
      brush_file_loader(graphics::d2d_context* devContext_p);

      std::unique_ptr<brush> operator()(const icontent_file* f,
                                        const id_t newID);

      private:
      graphics::d2d_context* m_ptr;
   };
}