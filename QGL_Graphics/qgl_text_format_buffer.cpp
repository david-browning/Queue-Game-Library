#include "pch.h"
#include "include/qgl_text_format_buffer.h"

static constexpr wchar_t* DEFAULT_FONT_FAMILY = L"Verdana";

static constexpr wchar_t* DEFAULT_FONT_LOCALE = L"en-us";

qgl::graphics::TEXT_FORMAT_BUFFER::TEXT_FORMAT_BUFFER() :
   m_fontSize(32.0f),
   m_tabStop(1.0f),
   m_fontWeight(DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_NORMAL),
   m_style(DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL),
   m_stretch(DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL),
   m_flowDirection(DWRITE_FLOW_DIRECTION::DWRITE_FLOW_DIRECTION_TOP_TO_BOTTOM),
   m_paragraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_NEAR),
   m_readingDirection(DWRITE_READING_DIRECTION::DWRITE_READING_DIRECTION_LEFT_TO_RIGHT),
   m_textAlignment(DWRITE_TEXT_ALIGNMENT_LEADING),
   m_wrapping(DWRITE_WORD_WRAPPING::DWRITE_WORD_WRAPPING_NO_WRAP),
   m_measuringMode(DWRITE_MEASURING_MODE::DWRITE_MEASURING_MODE_GDI_NATURAL),
   m_drawTextOptions(D2D1_DRAW_TEXT_OPTIONS::D2D1_DRAW_TEXT_OPTIONS_NONE),
   m_reserved1(0)
{
   MemorySet<wchar_t>(m_fontFamilyName, 0, MAX_FONT_CHARS);
   MemorySet<wchar_t>(m_fontLocaleName, 0, MAX_LANGUAGE_CHARS);
   MemoryCopy(m_fontFamilyName, DEFAULT_FONT_FAMILY, MemoryLength(DEFAULT_FONT_FAMILY));
   MemoryCopy(m_fontLocaleName, DEFAULT_FONT_LOCALE, MemoryLength(DEFAULT_FONT_LOCALE));
}

qgl::graphics::TEXT_FORMAT_BUFFER::TEXT_FORMAT_BUFFER(const TEXT_FORMAT_BUFFER& r) :
   m_fontSize(r.m_fontSize),
   m_tabStop(r.m_tabStop),
   m_fontWeight(r.m_fontWeight),
   m_style(r.m_style),
   m_stretch(r.m_stretch),
   m_flowDirection(r.m_flowDirection),
   m_paragraphAlignment(r.m_paragraphAlignment),
   m_readingDirection(r.m_readingDirection),
   m_textAlignment(r.m_textAlignment),
   m_wrapping(r.m_wrapping),
   m_measuringMode(r.m_measuringMode),
   m_drawTextOptions(r.m_drawTextOptions),
   m_reserved1(r.m_reserved1)
{
   MemoryCopy(m_fontFamilyName, r.family(), MAX_FONT_CHARS);
   MemoryCopy(m_fontLocaleName, r.locale(), MAX_LANGUAGE_CHARS);
}

qgl::graphics::TEXT_FORMAT_BUFFER::TEXT_FORMAT_BUFFER(TEXT_FORMAT_BUFFER && r) :
   m_fontSize(r.m_fontSize),
   m_tabStop(r.m_tabStop),
   m_fontWeight(r.m_fontWeight),
   m_style(r.m_style),
   m_stretch(r.m_stretch),
   m_flowDirection(r.m_flowDirection),
   m_paragraphAlignment(r.m_paragraphAlignment),
   m_readingDirection(r.m_readingDirection),
   m_textAlignment(r.m_textAlignment),
   m_wrapping(r.m_wrapping),
   m_measuringMode(r.m_measuringMode),
   m_drawTextOptions(r.m_drawTextOptions),
   m_reserved1(r.m_reserved1)
{
   MemoryCopy(m_fontFamilyName, r.family(), MAX_FONT_CHARS);
   MemoryCopy(m_fontLocaleName, r.locale(), MAX_LANGUAGE_CHARS);
}
