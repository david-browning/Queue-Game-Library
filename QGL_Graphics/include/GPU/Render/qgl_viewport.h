#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Interfaces/qgl_igraphics_device.h"
#include "include/qgl_window.h"

namespace qgl::graphics::gpu::render
{
   /*
    A viewport is the area of the render target surface to draw.
    */
   class QGL_GRAPHICS_API viewport
   {
      public:
      /*
       Construct a viewport that is the same width and height as the render 
       target.
       The viewport is then scaled to fit in the window.
       */
      viewport(const static_ptr_ref<graphics::igraphics_device> dev,
               const static_ptr_ref<graphics::window> wnd);

      /*
       Move constructor.
       */
      viewport(const viewport&) = default;

      viewport(viewport&&) = default;

      ~viewport() noexcept = default;

      /*
       Returns const a pointer to the D3D12 Viewport.
       */
      const D3D12_VIEWPORT* get() const noexcept;

      /*
       Returns a pointer to the D3D12 Viewport.
       */
      D3D12_VIEWPORT* get() noexcept;

      /*
       Swaps the contents of l and r.
       */
      inline friend void swap(viewport& l, 
                              viewport& r) noexcept
      {
         using std::swap;
         swap(l.m_viewport, r.m_viewport);
      }

      inline viewport& operator=(viewport r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      inline friend bool operator==(const viewport& l,
                                    const viewport& r) noexcept
      {
         return l.m_viewport == r.m_viewport;
      }

      private:
      D3D12_VIEWPORT m_viewport;
   };
}