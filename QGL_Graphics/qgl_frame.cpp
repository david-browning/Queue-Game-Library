#include "pch.h"
#include "include/qgl_frame.h"

qgl::graphics::frame::frame(const graphics_device& dev,
                            size_t frameIndex,
                            rtv_descriptor_heap& rtvHeap,
                            const DEPTH_STENCIL_BUFFER& buffer,
                            dsv_descriptor_heap& dsvHeap,
                            const window& wnd) :
   m_renderTarget(dev, frameIndex, rtvHeap),
   m_depthStencil(buffer, dev.com_d3d12_device(), wnd, frameIndex, dsvHeap),
   m_vp(dev.config(), wnd),
   m_scissor(m_vp)
{
   rtvHeap.insert(frameIndex, m_renderTarget);
   dsvHeap.insert(frameIndex, m_depthStencil);
}

qgl::graphics::frame::frame(frame&& r) :
   m_renderTarget(std::move(r.m_renderTarget)),
   m_depthStencil(std::move(r.m_depthStencil)),
   m_vp(std::move(r.m_vp)),
   m_scissor(std::move(r.m_scissor))
{
}
