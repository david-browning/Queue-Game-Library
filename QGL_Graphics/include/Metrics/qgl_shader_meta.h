#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/qgl_shader.h"

namespace qgl::graphics::metrics
{
   class QGL_GRAPHICS_API shader_meta
   {
      public:
      shader_meta(const content::shader* shdr);

      shader_meta(const shader_meta&);

      shader_meta(shader_meta&&);

      virtual ~shader_meta() noexcept;

      friend void swap(shader_meta& l, shader_meta& r) noexcept
      {
         using std::swap;
         swap(l.m_impl_p, r.m_impl_p);
      }

      shader_meta& operator=(shader_meta r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      private:
      struct impl;
      impl* m_impl_p = nullptr;
   };
}