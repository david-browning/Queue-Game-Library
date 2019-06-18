#include "pch.h"
#include "include\Content\Content-Buffers\qgl_text_format_buffer.h"

namespace qgl::content::buffers
{
   static constexpr const wchar_t* DEFAULT_FONT_FAMILY = L"Verdana";

   static constexpr const wchar_t* DEFAULT_FONT_LOCALE = L"en-us";

   TEXT_FORMAT_BUFFER::TEXT_FORMAT_BUFFER() :
      FontSize(32, 1),
      TabStop(1, 1),
      FontWeight(DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_NORMAL),
      Style(DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL),
      Stretch(DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL),
      FlowDirection(DWRITE_FLOW_DIRECTION::DWRITE_FLOW_DIRECTION_TOP_TO_BOTTOM),
      ParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_NEAR),
      ReadingDirection(DWRITE_READING_DIRECTION::DWRITE_READING_DIRECTION_LEFT_TO_RIGHT),
      TextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING),
      Wrapping(DWRITE_WORD_WRAPPING::DWRITE_WORD_WRAPPING_NO_WRAP),
      MeasuringMode(DWRITE_MEASURING_MODE::DWRITE_MEASURING_MODE_GDI_NATURAL),
      DrawTextOptions(D2D1_DRAW_TEXT_OPTIONS::D2D1_DRAW_TEXT_OPTIONS_NONE),
      Reserved1(0)
   {
      set_memory<wchar_t>(FontFamilyName, 0, MAX_FONT_CHARS);
      set_memory<wchar_t>(FontLocaleName, 0, MAX_LANGUAGE_CHARS);
      copy_elements(FontFamilyName,
                    DEFAULT_FONT_FAMILY,
                    mem_length(DEFAULT_FONT_FAMILY));
      copy_elements(FontLocaleName,
                    DEFAULT_FONT_LOCALE,
                    mem_length(DEFAULT_FONT_LOCALE));
   }
}