#include "pch.h"
#include "include/qgl_scissor.h"
#include "include/qgl_viewport.h"

qgl::graphics::scissor::scissor(const viewport& vp)
{
   auto& d3dViewport = vp.get();
   m_scissor.left = static_cast<LONG>(d3dViewport.TopLeftX);
   m_scissor.right = static_cast<LONG>(d3dViewport.TopLeftX + d3dViewport.Width);
   m_scissor.top = static_cast<LONG>(d3dViewport.TopLeftY);
   m_scissor.bottom = static_cast<LONG>(d3dViewport.TopLeftY + d3dViewport.Height);
}

qgl::graphics::scissor::scissor(const scissor& r):
   m_scissor(r.m_scissor)
{
}

qgl::graphics::scissor::scissor(scissor&& r) :
   m_scissor(std::move(r.m_scissor))
{
}

qgl::graphics::scissor::~scissor()
{
}
