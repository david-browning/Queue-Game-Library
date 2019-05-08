#include "pch.h"
#include "include/qgl_text_format.h"

qgl::graphics::text_format::text_format(const TEXT_FORMAT_BUFFER& format,
                                        const winrt::com_ptr<IDWriteFactory>& factory_p,
                                        const content::wcontent_item::str_t& name,
                                        const content::wcontent_item::id_t& id) :
   m_buffer(format),
   m_factory_p(factory_p),
   content_item(name, id,
                qgl::content::RESOURCE_TYPES::RESOURCE_TYPE_TEXT_FORMAT,
                qgl::content::CONTENT_LOADER_IDS::CONTENT_LOADER_ID_TEXT_FORMAT)
{
   m_create_text_format();
}

qgl::graphics::text_format::text_format(const text_format& r) :
   m_buffer(r.m_buffer),
   m_factory_p(r.m_factory_p),
   content_item(r)
{
   m_create_text_format();
}

qgl::graphics::text_format::text_format(text_format&& r) :
   m_factory_p(std::move(r.m_factory_p)),
   m_buffer(std::move(r.m_buffer)),
   content_item(std::move(r))
{
   m_create_text_format();
   r.m_factory_p = nullptr;
   r.m_format_p = nullptr;
}

void qgl::graphics::text_format::m_create_text_format()
{
   winrt::check_hresult(m_factory_p->CreateTextFormat(m_buffer.family(),
                                                      nullptr,
                                                      m_buffer.weight(),
                                                      m_buffer.style(),
                                                      m_buffer.stretch(),
                                                      m_buffer.size(),
                                                      m_buffer.locale(),
                                                      m_format_p.put()));

   winrt::check_hresult(m_format_p->SetFlowDirection(m_buffer.flow_direction()));
   winrt::check_hresult(m_format_p->SetIncrementalTabStop(m_buffer.tab_stop()));
   winrt::check_hresult(m_format_p->SetParagraphAlignment(m_buffer.paragraph_alignment()));
   winrt::check_hresult(m_format_p->SetReadingDirection(m_buffer.reading_direction()));
   winrt::check_hresult(m_format_p->SetTextAlignment(m_buffer.text_alignment()));
   winrt::check_hresult(m_format_p->SetWordWrapping(m_buffer.wrapping()));
}