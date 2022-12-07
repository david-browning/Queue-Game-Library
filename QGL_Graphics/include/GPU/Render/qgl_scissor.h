#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Render/qgl_viewport.h"

namespace qgl::graphics::gpu
{
   class scissor final
   {
      public:
      scissor(const viewport& vp)
      {
         auto& d3dViewport = vp.get();
         m_scissor.left = static_cast<LONG>(d3dViewport.TopLeftX);
         
         m_scissor.right = static_cast<LONG>(
            d3dViewport.TopLeftX + d3dViewport.Width);
         
         m_scissor.top = static_cast<LONG>(d3dViewport.TopLeftY);
         
         m_scissor.bottom = static_cast<LONG>(
            d3dViewport.TopLeftY + d3dViewport.Height);
      }

      scissor(const scissor&) = default;

      scissor(scissor&&) = default;

      ~scissor() noexcept = default;

      const D3D12_RECT& get() const noexcept
      {
         return m_scissor;
      }

      D3D12_RECT& get() noexcept
      {
         return m_scissor;
      }

      private:
      D3D12_RECT m_scissor;
   };
}