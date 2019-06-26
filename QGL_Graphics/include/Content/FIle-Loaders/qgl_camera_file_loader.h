#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::content
{
   class camera;
}

namespace qgl::graphics
{
   class window;
}

namespace qgl::content::loaders
{
   /*
    Functor for loading a camera from a camera content file.
    */
   class QGL_GRAPHICS_API camera_file_loader
   {
      public:
      camera_file_loader(const graphics::window* wnd_p,
                         graphics::d3d_device* dev_p);

      std::unique_ptr<camera> operator()(const icontent_file* f,
                                         const id_t newID) const;

      private:
      const graphics::window* m_wnd_p;
      graphics::d3d_device* m_dev_p;
   };
}