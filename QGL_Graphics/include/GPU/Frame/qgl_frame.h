#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/qgl_graphics_device.h"
#include "include/qgl_window.h"
#include "include/GPU/Descriptors/qgl_rtv_descriptor_heap.h"
#include "include/GPU/Descriptors/qgl_dsv_descriptor_heap.h"
#include "include/Content/Content-Buffers/qgl_depth_stencil_buffer.h"

namespace qgl::graphics::gpu::frame
{
   /*
    After creating a frame bind its frame_buffer() and frame_stencil() to 
    the RTV and DSV descriptor heaps.
    */
   class QGL_GRAPHICS_API frame
   {
      public:

      /*
       Creates a frame.
       */
      frame(graphics_device* dev,
            size_t frameIndex,
            const content::buffers::DEPTH_STENCIL_BUFFER* depthStencil,
            const rtv_descriptor_heap* rtvHeap,
            const dsv_descriptor_heap* dsvHeap,
            const window* wnd);

      /*
       Each frame must be bound to one RTV and DSV heap slot. Do not allow
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
      virtual ~frame();

      /*
       Returns a pointer to the render target.
       */
      const render_target* frame_buffer() const noexcept;

      /*
       Returns a pointer to the stencil.
       */
      const depth_stencil* frame_stencil() const noexcept;

      /*
       Returns a pointer to the viewport.
       The viewport defines the space of the render target that will be
       presented.
       */
      const viewport* frame_viewport() const noexcept;

      /*
       Returns a pointer to the scissor.
       By default, the scissor uses the same area as the viewport.
       */
      const scissor* frame_scissor() const noexcept;

      /*
       Acquires the necessary resources so rendering operations can be queued
       to this frame. Engines must call this before recording render commands.
       */
      void acquire(graphics_device* dev_p);

      /*
       Releases the frame's resources so it can be presented.
       Engines must call release before flushing the D3D11On12 context and
       presenting the frame on the swap chain.
       */
      void release(graphics_device* dev_p);

      /*
       Disposes of frame resources so the frame can be rebuilt.
       */
      void dispose(graphics_device* dev_p);

      private:
      struct impl;
      impl* m_impl_p;
   };
}