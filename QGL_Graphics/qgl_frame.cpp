#include "pch.h"
#include "include/GPU/Render/qgl_frame.h"

namespace qgl::graphics::gpu::render
{
   frame::frame(graphics::igraphics_device* dev,
                UINT frameIndex,
                content::depth_stencil* depthStencil_p,
                rtv_descriptor_heap* rtvHeap,
                const graphics::window* wnd) :
      m_viewport(dev->config(), wnd),
      m_scissor(&m_viewport),
      m_renderTarget(dev, rtvHeap, frameIndex),
      m_depthStencil_p(depthStencil_p)
   {
   }

   frame::frame(frame&& r) :
      m_viewport(std::move(r.m_viewport)),
      m_scissor(std::move(r.m_scissor)),
      m_renderTarget(std::move(r.m_renderTarget)),
      m_depthStencil_p(std::move(r.m_depthStencil_p))
   {

   }

   frame::~frame()
   {
   }

   const render_target* frame::frame_buffer() const noexcept
   {
      return &m_renderTarget;
   }

   render_target* frame::frame_buffer() noexcept
   {
      return &m_renderTarget;
   }

   const content::depth_stencil* frame::frame_stencil() const noexcept
   {
      return m_depthStencil_p;
   }

   content::depth_stencil* frame::frame_stencil() noexcept
   {
      return m_depthStencil_p;
   }

   const viewport* frame::frame_viewport() const noexcept
   {
      return &m_viewport;
   }

   viewport* frame::frame_viewport() noexcept
   {
      return &m_viewport;
   }

   const scissor* frame::frame_scissor() const noexcept
   {
      return &m_scissor;
   }

   scissor* frame::frame_scissor() noexcept
   {
      return &m_scissor;
   }

   void frame::acquire(graphics::igraphics_device* dev_p)
   {
      m_renderTarget.acquire_resources(dev_p->d3d11on12_device());
   }

   void frame::release(graphics::igraphics_device* dev_p)
   {
      m_renderTarget.release_resources(dev_p->d3d11on12_device());
   }

   void frame::dispose(graphics::igraphics_device* dev_p)
   {
      m_renderTarget.dispose(dev_p->d3d11on12_device());
   }
}