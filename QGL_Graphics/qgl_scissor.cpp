#include "pch.h"
#include "include/GPU/Render/qgl_scissor.h"

namespace qgl::graphics::gpu::render
{
   scissor::scissor(const static_ptr_ref<viewport> vp)
   {
      auto d3dViewport = vp->get();
      m_scissor.left = static_cast<LONG>(d3dViewport->TopLeftX);
      m_scissor.right = static_cast<LONG>(d3dViewport->TopLeftX + 
                                          d3dViewport->Width);
      m_scissor.top = static_cast<LONG>(d3dViewport->TopLeftY);
      m_scissor.bottom = static_cast<LONG>(d3dViewport->TopLeftY + 
                                           d3dViewport->Height);
   }
   
   const D3D12_RECT* scissor::get() const noexcept
   {
      return &m_scissor;
   }

   D3D12_RECT* scissor::get() noexcept
   {
      return &m_scissor;
   }
}