#include "pch.h"
#include "include/GPU/Stages/qgl_scissor.h"
#include "include/GPU/Stages/qgl_viewport.h"

namespace qgl::graphics::stages
{
   scissor::scissor(const viewport* vp)
   {
      auto d3dViewport = vp->get();
      m_scissor.left = static_cast<LONG>(d3dViewport->TopLeftX);
      m_scissor.right = static_cast<LONG>(d3dViewport->TopLeftX + 
                                          d3dViewport->Width);
      m_scissor.top = static_cast<LONG>(d3dViewport->TopLeftY);
      m_scissor.bottom = static_cast<LONG>(d3dViewport->TopLeftY + 
                                           d3dViewport->Height);
   }
}