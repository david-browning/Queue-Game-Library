#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics
{
   class wtext
   {
      wtext(std::wstring&& text, D2D_RECT_F&& rect) :
         m_text(std::forward<std::wstring>(text)),
         m_layout(std::forward<D2D_RECT_F>(rect))
      {

      }

      /*
       Returns the number of character long the text is.
       */
      size_t length() const noexcept
      {
         return m_text.size();
      }

      /*
       Returns a const pointer to a null-terminated, C-style wide string.
       */
      const wchar_t* c_str() const noexcept
      {
         return m_text.c_str();
      }

      /*
       Returns how big and where to draw the text.
       */
      const D2D_RECT_F& layout() const noexcept
      {
         return m_layout;
      }

      private:
      std::wstring m_text;
      D2D_RECT_F m_layout;
   };
}