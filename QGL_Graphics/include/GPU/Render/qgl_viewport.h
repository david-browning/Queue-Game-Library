#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Interfaces/qgl_igraphics_device.h"
#include "include/qgl_window.h"
#include "include/Content/qgl_device_configuration.h"

namespace qgl::graphics::gpu
{
   /*
    A viewport is the area of the render target surface to draw.
    */
   class viewport
   {
      public:
      /*
       Construct a viewport that is the same width and height as the render
       target.
       The viewport is then scaled to fit in the window.
       */
      viewport(const graphics_device_ptr& dev, const graphics::window& wnd)
      {
         auto windowWidth = wnd.width<float>();
         auto windowHeight = wnd.height<float>();
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

      /*
       Move constructor.
       */
      viewport(const viewport&) = default;

      viewport(viewport&&) = default;

      ~viewport() noexcept = default;

      /*
       Returns a const reference to the D3D12 Viewport.
       */
      const D3D12_VIEWPORT& get() const noexcept
      {
         return m_viewport;
      }

      /*
       Returns a const reference to the D3D12 Viewport.
       */
      D3D12_VIEWPORT& get() noexcept
      {
         return m_viewport;
      }

      /*
       Swaps the contents of l and r.
       */
      friend void swap(viewport& l,
         viewport& r) noexcept
      {
         using std::swap;
         swap(l.m_viewport, r.m_viewport);
      }

      viewport& operator=(viewport r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      viewport& operator=(viewport&&) = default;

      friend bool operator==(const viewport& l, const viewport& r) noexcept
      {
         return l.m_viewport == r.m_viewport;
      }

      private:
      D3D12_VIEWPORT m_viewport;
   };
}