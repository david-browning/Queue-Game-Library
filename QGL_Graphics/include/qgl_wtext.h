#pragma once
#include "qgl_graphics_include.h"

namespace qgl::graphics
{
   class LIB_EXPORT wtext : qgl::content::content_item
   {
      public:
      wtext(const std::wstring& text,
            const D2D_RECT_F& layout,
            const std::wstring& name,
            const content::content_id id);

      wtext(std::wstring&& text,
            const D2D_RECT_F& layout,
            const std::wstring& name,
            const content::content_id id);

      wtext(const std::wstring& text,
            D2D_RECT_F&& layout,
            const std::wstring& name,
            const content::content_id id);

      wtext(std::wstring&& text,
            D2D_RECT_F&& layout,
            const std::wstring& name,
            const content::content_id id);

      wtext(std::shared_ptr<std::wstring>& text_p,
            D2D_RECT_F& layout,
            const std::wstring& name,
            const content::content_id id);

      wtext(std::shared_ptr<std::wstring>& text_p,
            D2D_RECT_F&& layout,
            const std::wstring& name,
            const content::content_id id);

      wtext(const wtext& r) = default;

      wtext(wtext&& r) = default;

      virtual ~wtext() noexcept = default;

      /*
       Returns the number of character long the text is.
       */
      size_t length() const noexcept
      {
         return m_text_p->length();
      }

      /*
       Returns a const pointer to a null-terminated, C-style wide string.
       */
      const wchar_t* c_str() const noexcept
      {
         return m_text_p->c_str();
      }

      /*
       Returns how big and where to draw the text.
       */
      const D2D_RECT_F& layout() const noexcept
      {
         return m_rect;
      }

      private:
      #pragma warning(push)
      #pragma warning(disable: 4251)
      std::shared_ptr<std::wstring> m_text_p;
      #pragma warning(pop)
      D2D_RECT_F m_rect;
   };
}