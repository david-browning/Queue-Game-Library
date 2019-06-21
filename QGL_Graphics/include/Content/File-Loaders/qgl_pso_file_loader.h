#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/qgl_pso.h"

namespace qgl::content::loaders
{
   class QGL_GRAPHICS_API pso_file_loader
   {
      public:
      /*
       Do not allow the store to go out of scope.
       */
      pso_file_loader(graphics::d3d_device* dev_p,
                      icontent_store* store_p);

      std::unique_ptr<ipso> operator()(const icontent_file* f,
                                       id_t newID) const;

      private:
      icontent_store* m_store_p;
      graphics::d3d_device* m_dev_p;
   };
}