#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/Content-Descriptors/qgL_text_format_descriptor.h"

namespace qgl::graphics
{
   class text_format
   {
      public:
      text_format(const descriptors::text_format_descriptor& desc,
                  itext_factory* fact_p)
      {
         winrt::check_hresult(fact_p->CreateTextFormat(
            desc.font_family.data(),
            nullptr,
            static_cast<DWRITE_FONT_WEIGHT>(desc.weight),
            static_cast<DWRITE_FONT_STYLE>(desc.style),
            static_cast<DWRITE_FONT_STRETCH>(desc.stretch),
            static_cast<float>(desc.font_size),
            desc.font_locale.data(),
            m_fmt_p.put()));

         winrt::check_hresult(m_fmt_p->SetFlowDirection(
            static_cast<DWRITE_FLOW_DIRECTION>(desc.flow)));

         winrt::check_hresult(m_fmt_p->SetIncrementalTabStop(
            static_cast<float>(desc.tab_stop)));

         winrt::check_hresult(m_fmt_p->SetParagraphAlignment(
            static_cast<DWRITE_PARAGRAPH_ALIGNMENT>(
               desc.alignment)));

         winrt::check_hresult(m_fmt_p->SetReadingDirection(
            static_cast<DWRITE_READING_DIRECTION>(desc.read_direction)));

         winrt::check_hresult(m_fmt_p->SetTextAlignment(
            static_cast<DWRITE_TEXT_ALIGNMENT>(desc.alignment)));

         winrt::check_hresult(m_fmt_p->SetWordWrapping(
            static_cast<DWRITE_WORD_WRAPPING>(desc.wrapping)));
      }

      text_format(const text_format&) = default;

      text_format(text_format&&) = default;

      ~text_format() noexcept = default;

      /*
       Returns a const pointer to the D2D text format.
       */
      const IDWriteTextFormat* get() const noexcept
      {
         return m_fmt_p.get();
      }

      /*
       Returns a pointer to the D2D text format.
       */
      IDWriteTextFormat* get() noexcept
      {
         return m_fmt_p.get();
      }

      private:
      winrt::com_ptr<IDWriteTextFormat> m_fmt_p;
   };
}