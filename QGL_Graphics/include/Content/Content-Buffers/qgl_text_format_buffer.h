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

      TEXT_FORMAT_BUFFER(const TEXT_FORMAT_BUFFER&) = default;

      TEXT_FORMAT_BUFFER(TEXT_FORMAT_BUFFER&&) = default;

      ~TEXT_FORMAT_BUFFER() noexcept = default;

      DWRITE_FONT_WEIGHT weight() const;

      DWRITE_FONT_STYLE style() const;

      DWRITE_FONT_STRETCH stretch() const;

      float size() const;

      const wchar_t* locale() const;

      const wchar_t* family() const;

      DWRITE_FLOW_DIRECTION flow_direction() const;

      float tab_stop() const;

      DWRITE_PARAGRAPH_ALIGNMENT paragraph_alignment() const;

      DWRITE_READING_DIRECTION reading_direction() const;

      DWRITE_TEXT_ALIGNMENT text_alignment() const;

      DWRITE_WORD_WRAPPING wrapping() const;

      friend void swap(TEXT_FORMAT_BUFFER& l,
                       TEXT_FORMAT_BUFFER& r) noexcept;

      TEXT_FORMAT_BUFFER& operator=(TEXT_FORMAT_BUFFER r) noexcept;

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