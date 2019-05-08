#include "pch.h"
#include "include/qgl_wtext.h"

qgl::graphics::wtext::wtext(const std::wstring& text,
                            const D2D_RECT_F& layout,
                            const content::wcontent_item::str_t& name,
                            const content::wcontent_item::id_t& id) :
   m_rect(layout),
   content_item(name, id,
                qgl::content::RESOURCE_TYPES::RESOURCE_TYPE_STRING,
                qgl::content::CONTENT_LOADER_IDS::CONTENT_LOADER_ID_WTEXT)
{
   m_text_p = std::make_shared<std::wstring>(text);
}

qgl::graphics::wtext::wtext(std::wstring&& text,
                            const D2D_RECT_F& layout,
                            const content::wcontent_item::str_t& name,
                            const content::wcontent_item::id_t& id) :
   m_rect(layout),
   content_item(name, id,
                qgl::content::RESOURCE_TYPES::RESOURCE_TYPE_STRING,
                qgl::content::CONTENT_LOADER_IDS::CONTENT_LOADER_ID_WTEXT)
{
   m_text_p = std::make_shared<std::wstring>(std::move(text));
}

qgl::graphics::wtext::wtext(const std::wstring& text,
                            D2D_RECT_F&& layout,
                            const content::wcontent_item::str_t& name,
                            const content::wcontent_item::id_t& id) :
   m_rect(std::move(layout)),
   content_item(name, id,
                qgl::content::RESOURCE_TYPES::RESOURCE_TYPE_STRING,
                qgl::content::CONTENT_LOADER_IDS::CONTENT_LOADER_ID_WTEXT)
{
   m_text_p = std::make_shared<std::wstring>(text);
}

qgl::graphics::wtext::wtext(std::wstring&& text,
                            D2D_RECT_F&& layout,
                            const content::wcontent_item::str_t& name,
                            const content::wcontent_item::id_t& id) :
   m_rect(std::move(layout)),
   content_item(name, id,
                qgl::content::RESOURCE_TYPES::RESOURCE_TYPE_STRING,
                qgl::content::CONTENT_LOADER_IDS::CONTENT_LOADER_ID_WTEXT)
{
   m_text_p = std::make_shared<std::wstring>(std::move(text));
}

qgl::graphics::wtext::wtext(std::shared_ptr<std::wstring>& text_p, 
                            D2D_RECT_F& layout, 
                            const content::wcontent_item::str_t & name,
                            const content::wcontent_item::id_t & id) :
   m_rect(layout),
   m_text_p(text_p),
   content_item(name, id,
                qgl::content::RESOURCE_TYPES::RESOURCE_TYPE_STRING,
                qgl::content::CONTENT_LOADER_IDS::CONTENT_LOADER_ID_WTEXT)
{
}

qgl::graphics::wtext::wtext(std::shared_ptr<std::wstring>& text_p, 
                            D2D_RECT_F&& layout, 
                            const content::wcontent_item::str_t& name, 
                            const content::wcontent_item::id_t& id) :
   m_rect(std::move(layout)),
   m_text_p(text_p),
   content_item(name, id,
                qgl::content::RESOURCE_TYPES::RESOURCE_TYPE_STRING,
                qgl::content::CONTENT_LOADER_IDS::CONTENT_LOADER_ID_WTEXT)
{
}
