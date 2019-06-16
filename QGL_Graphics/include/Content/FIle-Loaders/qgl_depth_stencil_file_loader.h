#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics
{
   class igraphics_device;
   class window;
}

namespace qgl::graphics::gpu
{
   class dsv_descriptor_heap;
}

namespace qgl::graphics::gpu::render
{
   class depth_stencil;
}

namespace qgl::content::loaders
{
   class QGL_GRAPHICS_API depth_stencil_file_loader
   {
      public:
      depth_stencil_file_loader(
         graphics::igraphics_device* dev_p,
         const graphics::window* wnd_p,
         UINT frameIndex,
         const graphics::gpu::dsv_descriptor_heap* dsvHeap);

      std::unique_ptr<graphics::gpu::render::depth_stencil> operator()(
         const icontent_file* f,
         const id_t newID);

      private:
      graphics::igraphics_device* m_dev_p;
      const graphics::window* m_wnd_p;
      UINT m_frameIndex;
      const graphics::gpu::dsv_descriptor_heap* m_dsvHeap_p;
   };
}