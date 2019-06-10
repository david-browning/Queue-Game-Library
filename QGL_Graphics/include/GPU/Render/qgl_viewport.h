#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/Content-Buffers/qgl_graphic_config_buffer.h"

namespace qgl::content
{
   class graphics_config;
}

namespace qgl::graphics
{
   struct iwindow;
}

namespace qgl::graphics::gpu::render
{
   /*
    A viewport is the area of the render target surface to draw.
    */
   class QGL_GRAPHICS_API viewport
   {
      public:
      /*
       Construct a viewport that is the same width and height as the render.
       The viewport is then scaled to fit in the window.
       */
      viewport(const content::graphics_config* config,
               const graphics::iwindow* wnd);

      /*
       Move constructor.
       */
      viewport(const viewport&) = default;

      viewport(viewport&&) = default;

      virtual ~viewport() noexcept = default;

      const D3D12_VIEWPORT* get() const noexcept;

      D3D12_VIEWPORT* get() noexcept;

      friend void swap(viewport& l, viewport& r) noexcept
      {
         using std::swap;
         swap(l.m_viewport, r.m_viewport);
      }

      viewport& operator=(viewport r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      friend bool operator==(const viewport& l, const viewport& r) noexcept
      {
         return l.m_viewport == r.m_viewport;
      }

      private:
      D3D12_VIEWPORT m_viewport;
   };
}