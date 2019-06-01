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

      DWRITE_FONT_WEIGHT weight() const noexcept;

      DWRITE_FONT_STYLE style() const noexcept;

      DWRITE_FONT_STRETCH stretch() const noexcept;

      float size() const noexcept;

      const wchar_t* locale() const noexcept;

      const wchar_t* family() const noexcept;

      DWRITE_FLOW_DIRECTION flow_direction() const noexcept;

      float tab_stop() const noexcept;

      DWRITE_PARAGRAPH_ALIGNMENT paragraph_alignment() const noexcept;

      DWRITE_READING_DIRECTION reading_direction() const noexcept;

      DWRITE_TEXT_ALIGNMENT text_alignment() const noexcept;

      DWRITE_WORD_WRAPPING wrapping() const noexcept;

      friend void swap(TEXT_FORMAT_BUFFER& l,
                       TEXT_FORMAT_BUFFER& r) noexcept
      {
         using std::swap;
         swap(l.FontFamilyName, r.FontFamilyName);
         swap(l.FontLocaleName, r.FontLocaleName);
         swap(l.FontSize, r.FontSize);
         swap(l.TabStop, r.TabStop);
         swap(l.FontWeight, r.FontWeight);
         swap(l.Reserved1, r.Reserved1);
         swap(l.Style, r.Style);
         swap(l.Stretch, r.Stretch);
         swap(l.FlowDirection, r.FlowDirection);
         swap(l.ParagraphAlignment, r.ParagraphAlignment);
         swap(l.ReadingDirection, r.ReadingDirection);
         swap(l.TextAlignment, r.TextAlignment);
         swap(l.Wrapping, r.Wrapping);
         swap(l.MeasuringMode, r.MeasuringMode);
         swap(l.DrawTextOptions, r.DrawTextOptions);
      }

      TEXT_FORMAT_BUFFER& operator=(TEXT_FORMAT_BUFFER r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      wchar_t FontFamilyName[MAX_FONT_CHARS];

      /*
       An array of characters that contains the locale name.
       */
      wchar_t FontLocaleName[MAX_LANGUAGE_CHARS];
      //40

      /*
       The logical size of the font in DIP ("device-independent pixel") units. 
       A DIP equals 1/96 inch.
       Cannot be zero!
       */
      float FontSize;

      /*
       The fixed distance between two adjacent tab stops.
       Cannot be zero!
       */
      float TabStop;

      /*
       A value that indicates the font weight for the text.
       See https://tinyurl.com/d3d-font-weight
       */
      uint16_t FontWeight;

      /*
       Unused.
       */
      uint16_t Reserved1;
      //52

      /* Begin 1 Byte values */

      /*
       A value that indicates the font style for the text.
       See https://tinyurl.com/d2d-font-style
       */
      text_enum_t Style;

      /*
       A value that indicates the font stretch for the text.
       See https://tinyurl.com/d2d-font-stretch
       */
      text_enum_t Stretch;

      /*
       The paragraph flow direction.
       See https://tinyurl.com/dwrite-flow-direction
       */
      text_enum_t FlowDirection;
      
      /*
       The paragraph alignment option being set for a paragraph.
       See https://tinyurl.com/dwrite-para-align
       */
      text_enum_t ParagraphAlignment;
      
      /*
       The text reading direction 
       (for example, DWRITE_READING_DIRECTION_RIGHT_TO_LEFT for languages, 
       such as Arabic, that read from right to left) for a paragraph.
       See https://tinyurl.com/dwrite-read-direction
       */
      text_enum_t ReadingDirection;
      
      /*
       The text can be aligned to the leading or trailing edge of the layout 
       box, or it can be centered.
       See https://tinyurl.com/dwrite-text-align
       */
      text_enum_t TextAlignment;
      
      /*
       The word wrapping option being set for a paragraph.
       See https://tinyurl.com/dwrite-word-wrapping
       */
      text_enum_t Wrapping;
      
      
      text_enum_t MeasuringMode;
      
      
      text_enum_t DrawTextOptions;
   };
}