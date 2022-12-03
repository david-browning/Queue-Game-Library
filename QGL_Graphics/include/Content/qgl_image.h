#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/qgl_texture.h"
#include "include/GPU/Render/qgl_render_target.h"
#include "include/GPU/Memory/qgl_igpu_allocator.h"

namespace qgl::graphics
{
   class image
   {
      public:
      image(const texture&)
      {

      }

      image(const image&) = default;

      image(image&&) = default;

      virtual ~image() noexcept = default;

      ID2D1Image* get() noexcept
      {
         return m_img;
      }

      const ID2D1Image* get() const noexcept
      {
         return m_img;
      }

      private:
      ID2D1Image* m_img;
   };
}