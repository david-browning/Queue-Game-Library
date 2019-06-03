#include "pch.h"
#include "include/GPU/Render/qgl_frame.h"
#include "include/GPU/Render/qgl_scissor.h"
#include "include/GPU/Render/qgl_viewport.h"
#include "include/GPU/Render/qgl_render_target.h"
#include "include/GPU/Render/qgl_depth_stencil.h"

namespace qgl::graphics::gpu::render
{
   struct frame::impl
   {
      impl(graphics_device* dev,
           size_t frameIndex,
           const content::buffers::DEPTH_STENCIL_BUFFER* depthStencil,
           const rtv_descriptor_heap* rtvHeap,
           const dsv_descriptor_heap* dsvHeap,
           const window* wnd) :
         RenderTarget(dev, frameIndex, rtvHeap),
         Viewport(dev->config(), wnd),
         DepthStencil(depthStencil, 
                      dev, 
                      wnd, 
                      frameIndex, 
                      dsvHeap),
         Scissor(&Viewport)
      {
         
      }

      impl(const impl&) = default;

      impl(impl&&) = default;

      ~impl() = default;

      render_target RenderTarget;
      depth_stencil DepthStencil;
      viewport Viewport;
      scissor Scissor;
   };


   frame::frame(graphics_device* dev,
                size_t frameIndex,
                const content::buffers::DEPTH_STENCIL_BUFFER* depthStencil,
                const rtv_descriptor_heap* rtvHeap,
                const dsv_descriptor_heap* dsvHeap,
                const window* wnd) :
      m_impl_p(new impl(dev, 
                        frameIndex, 
                        depthStencil, 
                        rtvHeap,
                        dsvHeap,
                        wnd))
   {
   }

   frame::frame(frame&& r)
   {
      delete m_impl_p;
      m_impl_p = r.m_impl_p;
      r.m_impl_p = nullptr;
   }

   frame::~frame()
   {
      delete m_impl_p;
   }

   const render_target* frame::frame_buffer() const noexcept
   {
      return &m_impl_p->RenderTarget;
   }

   const depth_stencil* frame::frame_stencil() const noexcept
   {
      return &m_impl_p->DepthStencil;
   }

   const viewport* frame::frame_viewport() const noexcept
   {
      return &m_impl_p->Viewport;
   }

   const scissor* frame::frame_scissor() const noexcept
   {
      return &m_impl_p->Scissor;
   }

   void frame::acquire(graphics_device* dev_p)
   {
      m_impl_p->RenderTarget.acquire_resources(dev_p->d3d11on12_device());
   }

   void frame::release(graphics_device* dev_p)
   {
      m_impl_p->RenderTarget.release_resources(dev_p->d3d11on12_device());
   }

   void frame::dispose(graphics_device* dev_p)
   {
      m_impl_p->RenderTarget.dispose(dev_p->d3d11on12_device());
   }
}