#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Interfaces/qgl_igraphics_device.h"
#include "include/GPU/Descriptors/qgl_rtv_descriptor_heap.h"

#include "include/Content/qgl_depth_stencil.h"
#include "include/GPU/Render/qgl_scissor.h"
#include "include/GPU/Render/qgl_viewport.h"
#include "include/GPU/Render/qgl_render_target.h"
#include "include/qgl_window.h"


namespace qgl::graphics::gpu::render
{
   /*
    After creating a render bind its frame_buffer() and frame_stencil() to 
    the RTV and DSV descriptor heaps.
    */
   class QGL_GRAPHICS_API frame
   {
      public:
      /*
       Creates a render.
       depthStencil_p is content so the pointer is copied. It is the 
       responsibility of whoever provided the content to make sure it does not 
       get freed.
       */
      frame(graphics::igraphics_device* dev,
            UINT frameIndex,
            content::depth_stencil* depthStencil_p,
            gpu::rtv_descriptor_heap* rtvHeap,
            const graphics::window* wnd);

      /*
       Each render must be bound to one RTV and DSV heap slot. Do not allow
       copying frames.
       */
      frame(const frame&) = delete;

      /*
       Move constructor.
       */
      frame(frame&&);

      /*
       Destructor.
       */
      ~frame();

      /*
       Returns a pointer to the render target.
       */
      const render_target* frame_buffer() const noexcept;

      render_target* frame_buffer() noexcept;

      /*
       Returns a pointer to the stencil.
       */
      const content::depth_stencil* frame_stencil() const noexcept;

      content::depth_stencil* frame_stencil() noexcept;

      /*
       Returns a pointer to the viewport.
       The viewport defines the space of the render target that will be
       presented.
       */
      const viewport* frame_viewport() const noexcept;

      viewport* frame_viewport() noexcept;

      /*
       Returns a pointer to the scissor.
       By default, the scissor uses the same area as the viewport.
       */
      const scissor* frame_scissor() const noexcept;

      scissor* frame_scissor() noexcept;

      /*
       Acquires the necessary resources so rendering operations can be queued
       to this render. Engines must call this before recording render commands.
       */
      void acquire(graphics::igraphics_device* dev_p);

      /*
       Releases the render's resources so it can be presented.
       Engines must call release before flushing the D3D11On12 context and
       presenting the render on the swap chain.
       */
      void release(graphics::igraphics_device* dev_p);

      /*
       Disposes of render resources so the render can be rebuilt.
       */
      void dispose(graphics::igraphics_device* dev_p);

      private:
      render_target m_renderTarget;
      viewport m_viewport;
      scissor m_scissor;
      content::depth_stencil* m_depthStencil_p;
   };
}