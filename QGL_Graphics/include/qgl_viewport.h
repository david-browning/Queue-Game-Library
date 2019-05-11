#pragma once
#include "qgl_graphics_include.h"
#include "qgl_graphic_config_buffer.h"
#include "qgl_window.h"

namespace qgl::graphics
{
   class LIB_EXPORT viewport
   {
      public:
      viewport(const GRAPHICS_CONFIG_BUFFER& config,
               const window& wnd);

      viewport(const viewport&);

      viewport(viewport&&);

      virtual ~viewport();

      inline const auto& get() const
      {
         return m_viewport;
      }

      inline auto& get()
      {
         return m_viewport;
      }

      friend void swap(viewport& first, viewport& second) noexcept
      {
         using std::swap;
         swap(first.m_viewport, second.m_viewport);
      }

      viewport& operator=(viewport r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      private:
      D3D12_VIEWPORT m_viewport;
   };
}