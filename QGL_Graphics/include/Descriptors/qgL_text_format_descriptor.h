#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::descriptors
{
#pragma pack(push, 1)
   struct text_format_descriptor final
   {
      private:
      using text_enum_t = int8_t;

      public:
      static constexpr size_t MAX_LANGUAGE_CHARS = 8;
      static constexpr size_t MAX_FONT_CHARS = 32;

      constexpr text_format_descriptor()
      {

      }

      constexpr text_format_descriptor(const text_format_descriptor&) = default;

      constexpr text_format_descriptor(
         text_format_descriptor&&) noexcept = default;

      ~text_format_descriptor() noexcept = default;

      friend void swap(
         text_format_descriptor& r, text_format_descriptor& l) noexcept
      {
         using std::swap;
         swap(l.font_family, r.font_family);
         swap(l.font_locale, r.font_locale);
         swap(l.font_size, r.font_size);
         swap(l.tab_stop, r.tab_stop);
         swap(l.weight, r.weight);
         swap(l.reserved1, r.reserved1);
         swap(l.style, r.style);
         swap(l.stretch, r.stretch);
         swap(l.flow, r.flow);
         swap(l.paragraph_align, r.paragraph_align);
         swap(l.read_direction, r.read_direction);
         swap(l.alignment, r.alignment);
         swap(l.wrapping, r.wrapping);
         swap(l.measure_mode, r.measure_mode);
         swap(l.text_options, r.text_options);
      }

      text_format_descriptor& operator=(text_format_descriptor r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      /*
       Be sure to include the null terminator.
       */
      qgl::fixed_buffer<wchar_t, MAX_FONT_CHARS> font_family{ L"Veranda", 7 };

      /*
       An array of characters that contains the locale name.
       Be sure to include the null terminator.
       */
      qgl::fixed_buffer<wchar_t, MAX_LANGUAGE_CHARS> font_locale{ L"en-us", 5 };
      //40

      /*
       The logical size of the font in DIP ("device-independent pixel") units.
       A DIP equals 1/96 inch.
       Cannot be zero!
       */
      qgl::math::rational<int32_t> font_size{ 32, 1 };

      /*
       The fixed distance between two adjacent tab stops.
       Cannot be zero!
       */
      qgl::math::rational<int32_t> tab_stop{ 1,1 };

      /*
       A value that indicates the font weight for the text.
       See https://tinyurl.com/d3d-font-weight
       */
      uint16_t weight = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_NORMAL;

      /*
       Unused.
       */
      uint16_t reserved1 = 0;
      //52

      /* Begin 1 Byte values */

      /*
       A value that indicates the font style for the text.
       See https://tinyurl.com/d2d-font-style
       */
      text_enum_t style = DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL;

      /*
       A value that indicates the font stretch for the text.
       See https://tinyurl.com/d2d-font-stretch
       */
      text_enum_t stretch = DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL;

      /*
       The paragraph flow direction.
       See https://tinyurl.com/dwrite-flow-direction
       */
      text_enum_t flow =
         DWRITE_FLOW_DIRECTION::DWRITE_FLOW_DIRECTION_TOP_TO_BOTTOM;

      /*
       The paragraph alignment option being set for a paragraph.
       See https://tinyurl.com/dwrite-para-align
       */
      text_enum_t paragraph_align =
         DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_NEAR;

      /*
       The text reading direction
       (for example, DWRITE_READING_DIRECTION_RIGHT_TO_LEFT for languages,
       such as Arabic, that read from right to left) for a paragraph.
       See https://tinyurl.com/dwrite-read-direction
       */
      text_enum_t read_direction =
         DWRITE_READING_DIRECTION::DWRITE_READING_DIRECTION_LEFT_TO_RIGHT;

      /*
       The text can be aligned to the leading or trailing edge of the layout
       box, or it can be centered.
       See https://tinyurl.com/dwrite-text-align
       */
      text_enum_t alignment = DWRITE_TEXT_ALIGNMENT_LEADING;

      /*
       The word wrapping option being set for a paragraph.
       See https://tinyurl.com/dwrite-word-wrapping
       */
      text_enum_t wrapping = DWRITE_WORD_WRAPPING::DWRITE_WORD_WRAPPING_NO_WRAP;

      text_enum_t measure_mode =
         DWRITE_MEASURING_MODE::DWRITE_MEASURING_MODE_GDI_NATURAL;

      text_enum_t text_options =
         D2D1_DRAW_TEXT_OPTIONS::D2D1_DRAW_TEXT_OPTIONS_NONE;
   };
#pragma pack(pop)
}