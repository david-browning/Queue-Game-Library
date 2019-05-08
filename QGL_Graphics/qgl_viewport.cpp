#include "pch.h"
#include "include/qgl_viewport.h"

qgl::graphics::viewport::viewport(const GRAPHICS_CONFIG_BUFFER& config,
                                  const window& wnd)
{
   auto windowWidth = wnd.width();
   auto windowHeight = wnd.height();
   float viewWidthRatio = static_cast<float>(config.width()) / windowWidth;
   float viewHeightRatio = static_cast<float>(config.height()) / windowHeight;
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

qgl::graphics::viewport::viewport(const viewport& r) :
   m_viewport(r.m_viewport)
{
}

qgl::graphics::viewport::viewport(viewport&& r) :
   m_viewport(std::move(r.m_viewport))
{
}

qgl::graphics::viewport::~viewport()
{
}
