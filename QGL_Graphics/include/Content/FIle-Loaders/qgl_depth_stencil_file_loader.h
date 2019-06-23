#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/qgl_depth_stencil.h"

namespace qgl::graphics
{
   class igraphics_device;
   class window;
}

namespace qgl::graphics::gpu
{
   class dsv_descriptor_heap;
}

namespace qgl::content::loaders
{
   class QGL_GRAPHICS_API depth_stencil_file_loader
   {
      public:
      depth_stencil_file_loader(
         graphics::igraphics_device* dev_p,
         const graphics::gpu::dsv_descriptor_heap* dsvHeap,
         const graphics::window* wnd_p);

      std::unique_ptr<depth_stencil> operator()(const icontent_file* f,
                                                const id_t newID);

      private:
      graphics::igraphics_device* m_dev_p;
      const graphics::window* m_wnd_p;
      const graphics::gpu::dsv_descriptor_heap* m_dsvHeap_p;
   };
}