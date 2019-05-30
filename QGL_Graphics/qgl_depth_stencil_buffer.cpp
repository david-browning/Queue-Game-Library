#include "pch.h"
#include "include/Content/Content-Buffers/qgl_depth_stencil_buffer.h"

namespace qgl::graphics::content::buffers
{
   inline constexpr DEPTH_STENCIL_BUFFER::DEPTH_STENCIL_BUFFER() :
      m_depth(0),
      m_format(DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT),
      m_stencil(0),
      m_flags1(0)
   {

   }

   DXGI_FORMAT DEPTH_STENCIL_BUFFER::format() const
   {
      return static_cast<DXGI_FORMAT>(m_format);
   }

   const float& DEPTH_STENCIL_BUFFER::depth() const
   {
      return m_depth;
   }

   float& DEPTH_STENCIL_BUFFER::depth()
   {
      return m_depth;
   }

   const uint8_t& DEPTH_STENCIL_BUFFER::stencil() const
   {
      return m_stencil;
   }

   uint8_t& DEPTH_STENCIL_BUFFER::stencil()
   {
      return m_stencil;
   }
}