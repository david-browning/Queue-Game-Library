#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/Content-Descriptors/qgl_screen_space_descriptor.h"

namespace qgl::graphics
{
   class wtext
   {
      public:
      wtext(std::wstring& text, 
            descriptors::screen_space_descriptor& rect) :
         m_text(text),
         m_layout(rect)
      {

      }

      wtext(const wchar_t* text,
            descriptors::screen_space_descriptor& rect) :
         m_text(text),
         m_layout(rect)
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
      const descriptors::screen_space_descriptor& layout() const noexcept
      {
         return m_layout;
      }

      private:
      std::wstring m_text;
      descriptors::screen_space_descriptor m_layout;
   };
}