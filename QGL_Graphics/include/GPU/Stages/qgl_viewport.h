#pragma once
#include "include/qgl_graphics_include.h"
#include "include/qgl_window.h"
#include "include/Content/Content-Buffers/qgl_graphic_config_buffer.h"

namespace qgl::graphics::stages
{
   class QGL_GRAPHICS_API viewport
   {
      public:
      viewport(const content::buffers::GRAPHICS_CONFIG_BUFFER* config,
               const graphics::window* wnd);

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