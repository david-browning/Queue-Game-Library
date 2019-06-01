#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics::content
{
   class LIB_EXPORT wtext : qgl::content::content_item
   {
      public:
      wtext(const wchar_t* text,
            const D2D_RECT_F* layout,
            const wchar_t* name,
            const qgl::content::content_id id);

      wtext(const wtext& r);

      wtext(wtext&& r);

      virtual ~wtext() noexcept;

      /*
       Returns the number of character long the text is.
       */
      size_t length() const noexcept;

      /*
       Returns a const pointer to a null-terminated, C-style wide string.
       */
      const wchar_t* c_str() const noexcept;

      /*
       Returns how big and where to draw the text.
       */
      const D2D_RECT_F* layout() const noexcept;

      private:
      struct impl;
      impl* m_impl_p;
   };
}