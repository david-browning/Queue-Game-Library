#include "pch.h"
#include "include/GPU/Render/qgl_viewport.h"
#include "include/qgl_window.h"
#include "include/Content/qgl_graphics_config.h"

namespace qgl::graphics::gpu::render
{
   viewport::viewport(const static_ptr_ref<graphics::igraphics_device> dev,
                      const static_ptr_ref<graphics::window> wnd)
   {
      auto windowWidth = wnd->width();
      auto windowHeight = wnd->height();
      auto config = dev->config();
      float viewWidthRatio = static_cast<float>(
         config->width()) / windowWidth;
      float viewHeightRatio = static_cast<float>(
         config->height()) / windowHeight;
      float x = 1.0f;
      float y = 1.0f;

      if (viewWidthRatio < viewHeightRatio)
      {
         // The scaled image's height will fit to the viewport's height and 
         // its width will be smaller than the viewport's width.
         x = viewWidthRatio / viewHeightRatio;
      }
      else
      {
         // The scaled image's width will fit to the viewport's width and 
         // its height may be smaller than the viewport's height.
         y = viewHeightRatio / viewWidthRatio;
      }

      m_viewport.TopLeftX = windowWidth * (1.0f - x) / 2.0f;
      m_viewport.TopLeftY = windowHeight * (1.0f - y) / 2.0f;
      m_viewport.Width = x * windowWidth;
      m_viewport.Height = y * windowHeight;
   }

   const D3D12_VIEWPORT* viewport::get() const noexcept
   {
      return &m_viewport;
   }

   D3D12_VIEWPORT* viewport::get() noexcept
   {
      return &m_viewport;
   }
}