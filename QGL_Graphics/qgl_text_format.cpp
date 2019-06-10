#include "pch.h"
#include "include/Content/qgl_text_format.h"

namespace qgl::content
{
   struct text_format::impl
   {
      impl(const content::buffers::TEXT_FORMAT_BUFFER* format,
           IDWriteFactory* factory_p) :
         Format(nullptr),
         Buffer(*format)
      {
         create_text_format(factory_p);
      }

      impl(const impl&) = default;

      impl(impl&&) = default;

      virtual ~impl() noexcept = default;

      void create_text_format(IDWriteFactory* factory_p)
      {
         winrt::check_hresult(factory_p->CreateTextFormat(Buffer.family(),
                                                          nullptr,
                                                          Buffer.weight(),
                                                          Buffer.style(),
                                                          Buffer.stretch(),
                                                          Buffer.size(),
                                                          Buffer.locale(),
                                                          Format.put()));

         winrt::check_hresult(
            Format->SetFlowDirection(Buffer.flow_direction()));
         winrt::check_hresult(
            Format->SetIncrementalTabStop(Buffer.tab_stop()));
         winrt::check_hresult(
            Format->SetParagraphAlignment(Buffer.paragraph_alignment()));
         winrt::check_hresult(
            Format->SetReadingDirection(Buffer.reading_direction()));
         winrt::check_hresult(
            Format->SetTextAlignment(Buffer.text_alignment()));
         winrt::check_hresult(Format->SetWordWrapping(Buffer.wrapping()));
      }

      content::buffers::TEXT_FORMAT_BUFFER Buffer;
      winrt::com_ptr<IDWriteTextFormat> Format;
   };


   text_format::text_format(const content::buffers::TEXT_FORMAT_BUFFER* format,
                            IDWriteFactory* factory_p,
                            const wchar_t* name,
                            qgl::content::content_id id) :
      m_impl_p(new impl(format, factory_p)),
      content_item(
         name, id,
         qgl::content::RESOURCE_TYPES::RESOURCE_TYPE_DESCRIPTION,
         qgl::content::CONTENT_LOADER_IDS::CONTENT_LOADER_ID_TEXT_FORMAT)
   {
   }

   text_format::text_format(const text_format& r) :
      content_item(r)
   {
      delete m_impl_p;
      m_impl_p = new impl(*r.m_impl_p);
   }

   text_format::text_format(text_format&& r) :
      content_item(std::move(r))
   {
      delete m_impl_p;
      m_impl_p = r.m_impl_p;
      r.m_impl_p = nullptr;
   }

   text_format::~text_format() noexcept
   {
      delete m_impl_p;
      m_impl_p = nullptr;
   }

   const IDWriteTextFormat* text_format::get() const noexcept
   {
      return m_impl_p->Format.get();
   }

   IDWriteTextFormat* text_format::get() noexcept
   {
      return m_impl_p->Format.get();
   }

}