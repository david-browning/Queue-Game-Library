#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics::content::buffers
{
   static constexpr size_t MAX_LANGUAGE_CHARS = 8;
   static constexpr size_t MAX_FONT_CHARS = 32;

   using text_enum_t = int8_t;
   struct QGL_GRAPHICS_API TEXT_FORMAT_BUFFER
   {
      public:
      TEXT_FORMAT_BUFFER();

      TEXT_FORMAT_BUFFER(const TEXT_FORMAT_BUFFER&);

      TEXT_FORMAT_BUFFER(TEXT_FORMAT_BUFFER&&);

      ~TEXT_FORMAT_BUFFER() noexcept = default;

      DWRITE_FONT_WEIGHT weight() const
      {
         return static_cast<DWRITE_FONT_WEIGHT>(m_fontWeight);
      }

      DWRITE_FONT_STYLE style() const
      {
         return static_cast<DWRITE_FONT_STYLE>(m_style);
      }

      DWRITE_FONT_STRETCH stretch() const
      {
         return static_cast<DWRITE_FONT_STRETCH>(m_stretch);
      }

      float size() const
      {
         return m_fontSize;
      }

      const wchar_t* locale() const
      {
         return m_fontLocaleName;
      }

      const wchar_t* family() const
      {
         return m_fontFamilyName;
      }

      DWRITE_FLOW_DIRECTION flow_direction() const
      {
         return static_cast<DWRITE_FLOW_DIRECTION>(m_flowDirection);
      }

      float tab_stop() const
      {
         return m_tabStop;
      }

      DWRITE_PARAGRAPH_ALIGNMENT paragraph_alignment() const
      {
         return static_cast<DWRITE_PARAGRAPH_ALIGNMENT>(m_paragraphAlignment);
      }

      DWRITE_READING_DIRECTION reading_direction() const
      {
         return static_cast<DWRITE_READING_DIRECTION>(m_readingDirection);
      }

      DWRITE_TEXT_ALIGNMENT text_alignment() const
      {
         return static_cast<DWRITE_TEXT_ALIGNMENT>(m_textAlignment);
      }

      DWRITE_WORD_WRAPPING wrapping() const
      {
         return static_cast<DWRITE_WORD_WRAPPING>(m_wrapping);
      }

      private:
      wchar_t m_fontFamilyName[MAX_FONT_CHARS];
      wchar_t m_fontLocaleName[MAX_LANGUAGE_CHARS];
      //40

      //Cannot be zero!
      float m_fontSize;

      //Cannot be zero!
      float m_tabStop;
      uint16_t m_fontWeight;
      uint16_t m_reserved1;
      //52

      /* Begin 1 Byte values */
      text_enum_t m_style;
      text_enum_t m_stretch;
      text_enum_t m_flowDirection;
      text_enum_t m_paragraphAlignment;
      text_enum_t m_readingDirection;
      text_enum_t m_textAlignment;
      text_enum_t m_wrapping;
      text_enum_t m_measuringMode;
      text_enum_t m_drawTextOptions;
   };
}