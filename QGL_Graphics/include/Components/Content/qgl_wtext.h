#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Descriptors/qgl_screen_space_descriptor.h"

namespace qgl::components
{
   static constexpr guid WTEXT_GUID{ "39065D6AE325418BB9281900F84B26D5" };

   class wtext : public game_component<wtext>
   {
      public:
      wtext(std::wstring& text, 
            descriptors::screen_space_descriptor& rect,
            const game_update_functor<wtext>& updateFunctor) :
         m_text(text),
         m_layout(rect),
         component(WTEXT_GUID, updateFunctor)
      {

      }

      wtext(const wchar_t* text,
            descriptors::screen_space_descriptor& rect,
            const game_update_functor<wtext>& updateFunctor) :
         m_text(text),
         m_layout(rect),
         component(WTEXT_GUID, updateFunctor)
      {

      }

      wtext(const wtext&) = delete;

      wtext(wtext&&) noexcept = default;

      virtual ~wtext() noexcept = default;

      const std::wstring& str() const noexcept
      {
         return m_text;
      }

      std::wstring& str() noexcept
      {
         return m_text;
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