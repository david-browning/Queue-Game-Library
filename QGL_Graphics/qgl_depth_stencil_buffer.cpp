#include "pch.h"
#include "include/qgl_depth_stencil_buffer.h"

constexpr qgl::graphics::DEPTH_STENCIL_BUFFER::DEPTH_STENCIL_BUFFER() :
   m_depth(0),
   m_format(DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT),
   m_stencil(0),
   m_flags1(0)
{
}

constexpr qgl::graphics::DEPTH_STENCIL_BUFFER::DEPTH_STENCIL_BUFFER(const DEPTH_STENCIL_BUFFER& r) :
   m_depth(r.m_depth),
   m_format(r.m_format),
   m_stencil(r.m_stencil),
   m_flags1(r.m_flags1)
{
}

constexpr qgl::graphics::DEPTH_STENCIL_BUFFER::DEPTH_STENCIL_BUFFER(DEPTH_STENCIL_BUFFER && r) :
   m_depth(r.m_depth),
   m_format(r.m_format),
   m_stencil(r.m_stencil),
   m_flags1(r.m_flags1)
{
}
