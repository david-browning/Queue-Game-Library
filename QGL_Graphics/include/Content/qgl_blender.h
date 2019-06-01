#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/Content-Buffers/qgl_blender_buffer.h"

namespace qgl::graphics::content
{
   /*
    The blender provides the parameters describing how to blend pixels.
    */
   class QGL_GRAPHICS_API blender : qgl::content::content_item
   {
      public:
      /*
       Constructs a blender with default parameters.
       */
      blender(const wchar_t* name,
              qgl::content::content_id id);

      blender(const qgl::graphics::content::buffers::BLENDER_BUFFER* buff,
              const wchar_t* name,
              qgl::content::content_id id);

      blender(const blender&) = default;

      blender(blender&&) = default;

      virtual ~blender() noexcept = default;

      const D3D12_BLEND_DESC* description() const;

      UINT mask() const noexcept;

      friend void swap(blender& l, blender& r) noexcept
      {
         using std::swap;
         swap(l.m_desc, r.m_desc);
         swap(l.m_mask, r.m_mask);
      }

      blender& operator=(blender r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      private:
      D3D12_BLEND_DESC m_desc;
      UINT m_mask;
   };
}